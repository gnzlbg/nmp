# NMP - Non-blocking Message Passing (a C++14 MPI Wrapper)

This project is a proposal and a prototype implementation of a non-blocking
message passing interface. Its three main goals are:

  - fast and correct programs should be easy to write,
  - the user should remain in full control,
  - fail hard and early, but allow recovery if desired.

This goals motivate the following design decisions:

 - non-blocking _only_ interface (those who want to block can do so explicitly),
 - (insanely) strongly-typed,
 - generic (via concepts, polymorphic function objects, and tag-dispatching),
 - extensible (via non-member non-friend functions and ADL),
 - template meta-programming for zero abstraction penalty,
 - strong debug mode (with assertions that do not increase the algorithmic
   complexity)
 - insanely strong debug mode (with assertions that do increase the algorithmic
   complexity)
 - customizable error handling: exception vs hard failure
 - no external dependencies (except for the C++ standard library and `<mpi.h>`)

And the following implementation details about asynchronous message passing:

 - non-blocking API:
   - callback-based: offer a maximum performance call-back based asynchronous
     API following Boost.ASIO in the spirit of [N4045: Library Foundations for
     Asynchronous Operations](https://isocpp.org/files/papers/n4045.pdf).
   - future-based: offer an easy to use albeit _potentially slightly_ slower continuation
     based interface via `<futures>`. To minimize the impact on performance:
     - allow the user to customize the future type, and
     - provide stack allocated futures.

 - executors: the users should be able to choose the executor of the library as
   well as provide their own. Considering [N4046 - Executors and Asynchronous
   Operations](http://isocpp.org/files/papers/n4046.pdf) and the conflict between the
   run-time polymorphic nature of Google's proposal [N3785 - Executors and
   schedulers, revision
   3](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3785.pdf) and the
   goals of this library (performance and correctness) the executors that this
   library will target will be concept based.

 - serialization:
   - serialization of array-like data-structures (`<vector>`, `<stack>`,
     `<queue>`, `Eigen3::VectorXD`) of _trivial_ ypes with _standard layout_ should be as
     efficient as serializing a C pointer to a literal type with a size. This is
     already _very_ hard since in C++, e.g., a vector does not manage/own its
     memory (its allocator does) so avoiding extra allocations when using these
     is hard.
   - serialization of complex data-structures typically introduces a performance
     penalty since buffers have to be allocated. This performance penalty should
     _not_ be abstracted away. The users should remain in control here and manage
     the life-time of these buffers themselves. Utilities should be provided to 
     ease the management of these buffers.
