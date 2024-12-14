import Module from './build-debug/wasm_capillary_condensation.js';

const module = await Module()

console.log(module.get_volume(20.0).size())
