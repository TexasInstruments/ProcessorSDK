import numpy as np

def lut_in(nbit):
    if nbit <= 12: 
        luti = np.hstack([np.arange(0,4096,32, dtype='int'), 4096*np.ones(639-128, dtype='int')])
        return luti

    if 12 < nbit <= 20:
        step = 2048 // 2**(20-nbit)
        luti = np.hstack([np.arange(0,4096,32, dtype='int'), np.arange(4096, 2**nbit+1, step, dtype='int')])
        luti = np.hstack([luti, np.ones(639-len(luti), dtype='int') * luti[-1]])
        return luti

    if 20 < nbit <= 24:
        step1 =   32 * 2**(nbit-20)
        step2 = 2048 * 2**(nbit-20)
        luti = np.hstack([np.arange(0,4096*2**(nbit-20),step1, dtype='int'), np.arange(4096*2**(nbit-20), 2**nbit+1, step2, dtype='int')])
        return luti
