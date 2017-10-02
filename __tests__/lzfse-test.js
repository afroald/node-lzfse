/* eslint-env node, jest */

const lzfse = require('../lzfse');

describe('lzfse', () => {
  it('has a function', () => {
    expect(lzfse.compressSync).toEqual(expect.any(Function));
  });
});
