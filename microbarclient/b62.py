ALPHABET = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

def base62_encode(num, alphabet=ALPHABET):
    """Encode a number in Base X

    `num`: The number to encode
    `alphabet`: The alphabet to use for encoding
    """
    if (num == 0):
        return alphabet[0]
    arr = []
    base = len(alphabet)
    while num:
        rem = num % base
        num = num // base
        arr.append(alphabet[rem])
    arr.reverse()
    return ''.join(arr)
def mid_to_url(midint):
    '''
    >>> mid_to_url(3501756485200075)
    'z0JH2lOMb'
    >>> mid_to_url(3501703397689247)
    'z0Ijpwgk7'
    >>> mid_to_url(3501701648871479)
    'z0IgABdSn'
    >>> mid_to_url(3500330408906190)
    'z08AUBmUe'
    >>> mid_to_url(3500247231472384)
    'z06qL6b28'
    >>> mid_to_url(3491700092079471)
    'yCtxn8IXR'
    >>> mid_to_url(3486913690606804)
    'yAt1n2xRa'
    '''
    print midint
    midint = str(midint)[::-1]
    print midint 
    size = len(midint) / 7 if len(midint) % 7 == 0 else len(midint) / 7 + 1
    print size
    result = []
    for i in range(size):
        s = midint[i * 7: (i + 1) * 7][::-1]
        print s
        s = base62_encode(int(s))
        print s
        s_len = len(s)
        print s_len
        if i < size - 1 and len(s) < 4:
            s = '0' * (4 - s_len) + s
        print s
        result.append(s)
    result.reverse()
    return ''.join(result)

print "http://weibo.com/"+"1497390470/"+mid_to_url(3516027713260302)