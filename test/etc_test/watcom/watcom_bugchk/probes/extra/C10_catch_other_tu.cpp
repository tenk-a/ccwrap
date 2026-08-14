// Second TU for C10.  It must NOT throw: a throw odr-uses the constructor and
// makes the compiler emit the whole class, which hides the bug.
void g()
{
}
