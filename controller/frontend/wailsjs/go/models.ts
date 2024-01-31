export namespace core {
	
	export class SliderParams {
	    trackingMps: number;
	    panningRpm: number;
	
	    static createFrom(source: any = {}) {
	        return new SliderParams(source);
	    }
	
	    constructor(source: any = {}) {
	        if ('string' === typeof source) source = JSON.parse(source);
	        this.trackingMps = source["trackingMps"];
	        this.panningRpm = source["panningRpm"];
	    }
	}
	export class SliderStatus {
	    params: SliderParams;
	    activeProgram: boolean;
	    state: number;
	    percentComplete: number;
	
	    static createFrom(source: any = {}) {
	        return new SliderStatus(source);
	    }
	
	    constructor(source: any = {}) {
	        if ('string' === typeof source) source = JSON.parse(source);
	        this.params = this.convertValues(source["params"], SliderParams);
	        this.activeProgram = source["activeProgram"];
	        this.state = source["state"];
	        this.percentComplete = source["percentComplete"];
	    }
	
		convertValues(a: any, classs: any, asMap: boolean = false): any {
		    if (!a) {
		        return a;
		    }
		    if (a.slice) {
		        return (a as any[]).map(elem => this.convertValues(elem, classs));
		    } else if ("object" === typeof a) {
		        if (asMap) {
		            for (const key of Object.keys(a)) {
		                a[key] = new classs(a[key]);
		            }
		            return a;
		        }
		        return new classs(a);
		    }
		    return a;
		}
	}

}

