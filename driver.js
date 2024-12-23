import Module from './build-release-emscripten/wasm_capillary_condensation.js';

const module = await Module()

let componentInstance = new module.Component("Custom component", 1100.0, 150.17e-3, 0.267)
console.log(componentInstance.name_readonly)
console.log(componentInstance.density_readonly)
console.log(componentInstance.molecular_weight_readonly)
console.log(componentInstance.molar_volume_readonly)
console.log(componentInstance.p_sat(0.0))

let r_part = 14.0e-9;
let neck_fa = 10.0;
let ca = 0.0;
let surface_tension = 0.05
let temperature = 300.0;
let saturation = 1.0
let dt = 0.001
let t_tot = 1.0

let result = module.run_single_component_capillary_condensation(componentInstance, r_part, neck_fa, ca, surface_tension, temperature, saturation, dt, t_tot)

for (let i = 0; i < result.size(); i ++) {
    console.log(result.get(i))
}

console.log("Condensation engine built from " + module.get_condensation_engine_tag())

componentInstance.delete()


// const TEG = module.TEG
//
// console.log(TEG.density_readonly)

// module.do_stuff()
