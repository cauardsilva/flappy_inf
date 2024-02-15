float FitToRange(float n, float range_min, float range_max)
{
    return n > range_max ? range_max : n < range_min ? range_min : n;
}