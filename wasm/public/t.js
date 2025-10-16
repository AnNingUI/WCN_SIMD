// 保存为 convert.js
import fs from 'fs';
// 读取 wasm 二进制文件
const wasmBuffer = fs.readFileSync('./WCN_SIMD.wasm');
// 转换为 Base64 字符串
const base64 = wasmBuffer.toString('base64');
// 生成可内嵌到 JS 的代码（导出一个获取 Uint8Array 的函数）
const output = `export const wasmBase64 = '${base64}';\n`;
fs.writeFileSync('wasm-embedded.js', output);