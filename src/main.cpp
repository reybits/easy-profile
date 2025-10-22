#include "any.h"
#include "def.h"
#include "strong.h"
#include "variant.h"

int main()
{
    any::test();
    variant::test();
    strong::test();
    def::test();

    return 0;
}
