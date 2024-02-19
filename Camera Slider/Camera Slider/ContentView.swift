//
//  ContentView.swift
//  Camera Slider
//
//  Created by John Jones on 2/18/24.
//

import SwiftUI

struct ContentView: View {
    @ObservedObject var slider = SliderController()
    @State var trackingMps: Double = 1.5
    @State var panningRpm: Double = 2
    @State var percentDistance: Double = 1
    @State var rotationDirection: Bool = true
    
    var body: some View {
        VStack {
            ProgressView("Percent Complete", value: slider.percentComplete, total: 1)
                .disabled(!slider.activeProgram)
                .opacity(slider.activeProgram ? 1 : 0)
            
            Text(slider.mode.description)
                .padding(EdgeInsets(top: 0, leading: 0, bottom: 40, trailing: 0))
            
            Divider()
            
            Text("Tracking Speed: \(String(format: "%.2f", trackingMps))")
            Slider(value: $trackingMps, in: 0...3)
            
            Text("Panning Speed: \(String(format: "%.2f", panningRpm))")
            Slider(value: $panningRpm, in: 0...3)
            
            HStack {
                Button(action: {
                    self.rotationDirection = false
                }) {
                    Image(systemName: "arrow.left")
                        .resizable()
                        .frame(width: 30, height: 30)
                        .foregroundColor(!rotationDirection ? .blue : .black)
                }
                .padding()
                .background(Color.white)
                .cornerRadius(8)
                
                Button(action: {
                    self.rotationDirection = true
                }) {
                    Image(systemName: "arrow.right")
                        .resizable()
                        .frame(width: 30, height: 30)
                        .foregroundColor(rotationDirection ? .blue : .black)
                }
                .padding()
                .background(Color.white)
                .cornerRadius(8)
            }
            
            Text("Percent Completion: \(String(format: "%.2f", percentDistance))")
            Slider(value: $percentDistance, in: 0...1)
            
            Spacer()
            
            Button {
                slider.send(params: SliderParams(
                    speed: SliderSpeed(
                        trackingMps: Float(self.trackingMps),
                        panningRpm: Float(self.panningRpm)),
                    percentDistance: Float(self.percentDistance),
                    rotationDirection: !self.rotationDirection
                ))
            } label: {
                Text("Start")
                    .font(.headline)
                    .foregroundColor(.white)
                    .padding()
                    .frame(maxWidth: .infinity)
                    .background(Color.blue)
                    .cornerRadius(8)
            }
            .disabled(slider.activeProgram)

        }
        .padding()
        .disabled(!slider.connected)
        .onAppear {
            slider.start()
        }
        //TODO error
    }
}

#Preview {
    ContentView()
}
