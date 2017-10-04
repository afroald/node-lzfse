/* eslint-env node, jest */

const fs = require('fs');
const path = require('path');
const { promisify } = require('util');
const lzfse = require('../lzfse');

const readFile = promisify(fs.readFile);

describe('lzfse compressSync', () => {
  it('throws error when input buffer is empty', async () => {
    const input = Buffer.from([]);

    expect(() => {
      lzfse.compressSync(input);
    }).toThrow('Input is empty');
  });
});

describe('lzfse decompressSync', () => {
  it('throws an error when input is empty', () => {
    const input = Buffer.from([]);

    expect(() => {
      lzfse.decompressSync(input);
    }).toThrow('Input is empty');
  });
});

describe('lzfse', () => {
  it('correctly compresses and decompresses', () => {
    const files = [
      path.resolve(__dirname, 'data/text-567b.txt'),
      path.resolve(__dirname, 'data/text-100kb.txt'),
    ];

    return Promise.all(files.map(file => (async () => {
      const input = await readFile(file);

      const compressed = lzfse.compressSync(input);
      expect(Buffer.byteLength(compressed)).toBeGreaterThan(0);

      const decompressed = lzfse.decompressSync(compressed);
      expect(decompressed.equals(input)).toBe(true);
    })()));
  });

  it('correctly compresses and decompresses uncompressable files', () => {
    const files = [
      path.resolve(__dirname, 'data/text-1b.txt'),
      path.resolve(__dirname, 'data/whitenoise.wav'),
    ];

    return Promise.all(files.map(file => (async () => {
      const input = await readFile(file);

      const compressed = lzfse.compressSync(input);
      expect(Buffer.byteLength(compressed)).toBeGreaterThan(0);

      const decompressed = lzfse.decompressSync(compressed);
      expect(decompressed.equals(input)).toBe(true);
    })()));
  });
});
