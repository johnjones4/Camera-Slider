//
//  SliderController.swift
//  Camera Slider
//
//  Created by John Jones on 2/18/24.
//

import Foundation
import CoreBluetooth

let ServiceUUID = CBUUID(string: "A7F7A363-1642-475A-8252-8BCE4C4289E9")
let ProgramCharacteristicUUID = CBUUID(string: "314A2B2A-EB2C-4D05-87CD-475DBA39BC0F")
let StateCharacteristicUUID = CBUUID(string: "9C282BA8-2BCE-4DF7-AA80-ED014C471B21")

enum SliderMode: Int32 {
    case Unknown = -1
    case Homing = 0
    case Idle = 1
    case Program = 2
    
    var description : String {
        switch self {
        case .Unknown:
            return "Unknown"
        case .Homing:
            return "Homing"
        case .Idle:
            return "Idle"
        case .Program:
            return "Program"
        }
    }
}

struct SliderSpeed {
    let trackingMps: Float32
    let panningRpm: Float32
    
    var data : Data {
        var data = Data()
        data.append(trackingMps.data)
        data.append(panningRpm.data)
        return data
    }
}

struct SliderParams {
    let speed: SliderSpeed
    let percentDistance: Float
    let rotationDirection: Bool
    
    var data : Data {
        var data = Data()
        data.append(speed.data)
        data.append(percentDistance.data)
        data.append(rotationDirection.data)
        return data
    }
}

class SliderController : NSObject, ObservableObject {
    static let main = SliderController()
    
    var manager: CBCentralManager!
    var slider: CBPeripheral?
    var programCharacteristic: CBCharacteristic?
    var stateCharacteristic: CBCharacteristic?
    
    @Published var mode : SliderMode = .Unknown
    @Published var activeProgram: Bool = false
    @Published var percentComplete: Float = 0
    @Published var error: Error?
    @Published var connected: Bool = false
    
    override init() {
        super.init()
        manager = CBCentralManager(delegate: self, queue: nil)
    }

    func start() {
        self.connected = false
        
        if manager.isScanning {
            manager.stopScan()
        }
        
    }
    
    func send(params: SliderParams) {
        guard let slider = self.slider else { return }
        guard let programCharacteristic = self.programCharacteristic else {return}
        
        var data = Data()
        data.append(0xA4)
        data.append(params.data)
        
        slider.writeValue(data, for: programCharacteristic, type: .withoutResponse)
    }
}

extension SliderController: CBCentralManagerDelegate {
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        if central.state == .poweredOn && !self.connected {
            manager.scanForPeripherals(withServices: [ServiceUUID], options: nil)
        }
    }
    
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
        self.slider = peripheral
        peripheral.delegate = self
        manager.connect(peripheral, options: nil)
        manager.stopScan()
    }

    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        peripheral.discoverServices(nil)
    }

    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        if let _error = error {
            self.error = _error
        }
        self.start()
    }

    func centralManager(_ central: CBCentralManager, didFailToConnect peripheral: CBPeripheral, error: Error?) {
        if let _error = error {
            self.error = _error
        }
        self.start()
    }
}

extension SliderController: CBPeripheralDelegate {
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        if let _error = error {
            self.error = _error
            return
        }
        
        guard let services = peripheral.services else { return }

        guard let service = services.first else {return}
        peripheral.discoverCharacteristics(nil, for: service)
    }

    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        if let _error = error {
            self.error = _error
            return
        }
        
        guard let characteristics = service.characteristics else { return }
        for characteristic in characteristics {
            switch characteristic.uuid.uuidString {
            case ProgramCharacteristicUUID.uuidString:
                self.programCharacteristic = characteristic
            case StateCharacteristicUUID.uuidString:
                self.stateCharacteristic = characteristic
            default:
                NSLog("Unknown characteristic \(characteristic.uuid.uuidString)")
            }
        }
        
        if let stateCharacteristic = self.stateCharacteristic {
            self.slider?.setNotifyValue(true, for: stateCharacteristic)
        }
        
        self.connected = true
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        if characteristic != stateCharacteristic {
            return
        }
        
        guard let data = characteristic.value else {
            return
        }
        
        let activeProgram = data.bool(at: 16)
        self.activeProgram = activeProgram
        
        if let newMode = SliderMode(rawValue: data.int32(at: 20)) {
            self.mode = newMode
        }
        
        self.percentComplete = data.float(at: 24)
    }
}

extension Data {
    private func subvalue<T>(at: Data.Index) -> T {
        let end = at + MemoryLayout<T>.size
        let bytes = self.subdata(in: at..<end)
        return bytes.withUnsafeBytes { $0.load(as: T.self) }
    }
    
    func float(at: Data.Index) -> Float32 {
        return subvalue(at: at)
    }
    
    func int32(at: Data.Index) -> Int32 {
        return subvalue(at: at)
    }
    
    func bool(at: Data.Index) -> Bool {
        let val : Int32 =  subvalue(at: at)
        return val == 1
    }
}

extension Float32 {
    var data : Data {
        return withUnsafeBytes(of: self) { Data($0) }
    }
}

extension Bool {
    var data : Data {
        let val : Int32 = self ? 1 : 0
        return withUnsafeBytes(of: val) { Data($0) }
    }
}

