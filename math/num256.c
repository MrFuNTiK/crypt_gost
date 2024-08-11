

#include <assert.h>
#include <internal_include/math/math.h>

void CrptMath_256Number_add(CrptMath_256Number* res,
                            CrptMath_256Number* first,
                            CrptMath_256Number* second)
{
    assert(res);
    assert(first);
    assert(second);

    int carry = 0;
    for( int i = 0; i < NUMBER_256_SIZE_UI64; ++i )
    {
        res->d.ui64[i] = first->d.ui64[i] + second->d.ui64 + carry;
        carry = ( res->d.ui64[i] < first->d.ui64[i] ) ? 1 : 0;
    }
}
