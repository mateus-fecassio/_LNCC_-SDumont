__kernel void raiz( __global float * v )
{
   int id = get_global_id(0);
   v[id] = sqrt( v[id] );
}
