### Overview

The intent is to use the composability of **Theil's U** (or the uncertainty
coefficient [^1]) to draw a map of how entropy (or information) flows in a given
program. There are various levels of granularity we can compose at, and for
now, we attempt to use individual `LLVM IR` instructions. In this note, we pen
some thoughts about a trio of specific `LLVM IR` instructions, and how we
believe they work in relation the composability of Theil's U.

### Approach

For each `composible unit` (in our current approach, `LLVM IR` instruction), we
can compute **Theil's U** by directly computing entropy, and we do so
empirically. We fuzz (randomly generate) required inputs for each `composible
unit`, execute it, and record the respective output. If we assume *I* is the
set of input observations, and *O* is the set of output observations, then we
can compute the following:
* `P(I=i)`, where $i$ is some input value in $I$
* `P(O=o)`, where $o$ is some output value in $O$
* `H(I)`
* `H(O)`
* `H(I|O)`
* `U(I|O)`

### Instructions of Interest

The instructions we discuss especially are the following:
* `extractelement` [^2] - takes as input a `vector` and an `integer` value;
  extracts a scalar element of the vector at the index specified by the integer
  value
* `extractvalue` [^3] - same as `extractelement`, but the first operand is an
  `aggregate` type value (either `array` or `struct`)
* `getelementpointer` [^4] - takes an arbitrary `aggregate` structure, and an
  arbitrary number of operands to be used as indices; the first index is used
  to obtain a value inside the aggregate operand, the next index is used to
  index into the previously obtained type, and so on and so forth, essentially
  indexing aggregate structures within aggregate structures; importantly, this
  operation only computes an address, does not access or retrieve to element
  (i.e. it does not dereference the computed address), which is usually done
  via a subsequent `load` operation

We consider these operations in term of our approach. `extractelement` and
`extractvalue` seem rather similar on the surface, so we shall consider them
sufficiently similar to only discuss `extractelement` in detail. Taking the
instruction in isolation, suppose we have a vector of type `<n * im>`, where
`n` represents the number of vector elements, and `m` represents the bit-width
of the integer elements (e.g., `i8` is an element of 8 bits wide). Intuitively,
when we perform an `extractelement`, we throw away all elements but the one
selected, which would be equivalent to retaining `1/n` entropy.

#### Problem 1 - consecutive `extractelements`

The first issue is when we have consecutive `extractelement` operations.
Suppose we have `n` such operations, each taking a consecutive index.
Essentially, we are extracting all elements in isolation. Again, thinking about
intuition, we do not lose any information - we have all the data contained
within the original vector as we had before, and the additive nature of entropy
agrees with us. However, if we consider Theil's U composability, this is
multiplicative, leading to a total entropy retainment at the end of such a
block of `1/(n^n)`.

A possible explanation for this is that Theil's U is only composable when the
output of a prior `composible unit` is used as an input to a subsequent one.

#### Problem 2 - state entropy

An operation like `extractelement` does not remove any information from the
state itself (assuming it does not overwrite the memory location where the
input `vector` operand is held, which is unlikely). So arguably, such an
operation would not lose any entropy. We have yet to consider recording state
in our approach, but perhaps this issue shows that we should consider doing
that.

#### Problem 3 - direct entropy calculation

Let's go back to calculating entropy mathematically, based on input and output
probabilities. Suppose we have a vector of type `<2^2 * i32>` elements, and an
`index` of type `i8`. There are `2^2` valid values for the `index` variable,
equal to the total number of elements in the vector. The remainder `2^8 - 2^2`
values will all yield the same `poison` value ("the same `poison`" is
incorrect, I think, but we use it here for simplicity). We could then perform
the required computations to get the exact `U` value, but it should be roughly
`1 - (2^2 / 2^8)`. Probably.

#### Problem 4 - GEP versus the others

Based on empirical observations, it seems `LLVM IR` greatly prefers using `GEP`
(`getelementpointer`), perhaps due to it being more generic than the others, as
it works just with raw pointers. However, our intuition says that computing an
address doesn't affect the state, nor does it remove or select any information.
It just performs some arithmetic. A subsequent `load` is used to the extract
the required element at the address computed by `GEP`. Thus, again, looking at
instructions in isolation might not be the right approach, as both
`extractelement` and `extractvalue` can easily (and likely) be written as a
`GEP` plus a `load`. But in the former case, we have entropy loss, while in the
latter, we do not (a `load` in isolation also does not lose any entropy, in our
stateless approach).

[^1]: https://en.wikipedia.org/wiki/Uncertainty\_coefficient
[^2]: https://llvm.org/docs/LangRef.html#extractelement-instruction
[^3]: https://llvm.org/docs/LangRef.html#extractvalue-instruction
[^4]: https://llvm.org/docs/LangRef.html#getelementptr-instruction
