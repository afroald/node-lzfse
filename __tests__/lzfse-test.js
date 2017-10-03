/* eslint-env node, jest */

const fs = require('fs');
const path = require('path');
const { promisify } = require('util');
const lzfse = require('../lzfse');

const readFile = promisify(fs.readFile);

const testFile = path.resolve(__dirname, 'data/text-100kb.txt');
const smallUncompressableTestFile = path.resolve(__dirname, 'data/text-1b.txt');
const uncompressableTestFile = path.resolve(__dirname, 'data/whitenoise.wav');

describe('lzfse compression', () => {
  it('compresses', async () => {
    const input = await readFile(testFile);
    const output = lzfse.compressSync(input);

    expect(Buffer.byteLength(output)).toBeGreaterThan(0);
    expect(Buffer.byteLength(output)).toBeLessThanOrEqual(Buffer.byteLength(input));
  });

  it('throws error when input buffer is empty', async () => {
    const input = Buffer.from([]);

    expect(() => {
      lzfse.compressSync(input);
    }).toThrow('Input is empty');
  });

  it('doesn\'t break on uncompressable files', () => {
    const testPromises = [smallUncompressableTestFile, uncompressableTestFile].map((file) => {
      return async () => {
        const input = await readFile(file);
        let output;

        expect(() => {
          output = lzfse.compressSync(input);
        }).not.toThrow();

        expect(Buffer.byteLength(output)).toBeGreaterThanOrEqual(Buffer.byteLength(input));
      };
    });

    return Promise.all(testPromises);
  });
});

describe('lzfse decompression', () => {
  it('decompresses', async () => {
    const input = await readFile(testFile);
    const compressed = lzfse.compressSync(input);
    const uncompressed = lzfse.decompressSync(compressed);

    expect(input.equals(uncompressed)).toBe(true);
  });

  it('throws an error when input is empty', () => {
    const input = Buffer.from([]);

    expect(() => {
      lzfse.decompressSync(input);
    }).toThrow('Input is empty');
  });
});
