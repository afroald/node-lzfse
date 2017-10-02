const bindings = require('bindings');

const lzfse = bindings('lzfse');

module.exports = {
  compressSync: lzfse.compressSync,
  decompressSync: lzfse.decompressSync,
};
