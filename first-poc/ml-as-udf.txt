Towards a ML UDF for UC2
========================

Goal of this activity is to establish a rasdaman User-Defined Function (UDF) for ML purposes in UC2. 
A UDF is executable code residing on the rasdaman datacube server engine that gets invoked from 
within a query sent to this server. When this happens the server dynamically links the code into
the execution engine so that the corresponding external code function can be invoked and executed.

The server API for attaching external code is a C++ library, combined with automatic stub code 
generation. Both n-D arrays and scalars (numbers, strings) can be passed into a UDF invocation and 
returned by a UDF as the function result. The central class for arrays is the rasdaman API class 
r_GMArray.

As part of the JacobsU (JU) work in FAIRiCUBE a UDF mechanism will be provided so that (py)torch can 
be invoked from within a query; this in the sequel is referred to as "the torch UDF". WUR then can 
use this UDF for its specific ML work.

Workplan (Draft)
----------------
The following work items and steps are considered initially (to be discussed and refined while going).

- WUR: provide a documented working example of pytorch code which reads from files as traditionally;  
  this allows JacobsU to familiarize with the particular API and function use
- JU: in this working example, replace the file reading code by rasdaman datacube access
- WUR+JU: establish the interface definition of the torch UDF
- JU: move the WUR torch invocation code example into a server-side UDF
- WUR: evaluate the UDF

Below these steps are documented.

Working WUR pytorch example
---------------------------


Pytorch with datacube access
----------------------------

Torch UDF interface definition
------------------------------

Torch as UDF
------------

Evaluation
----------

