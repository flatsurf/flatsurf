#include <boost/type_erasure/any_cast.hpp>

#include <fmt/format.h>

#include "../flatsurf/fmt.hpp"

#include "impl/asserted_lengths.hpp"

#include "util/assert.ipp"

template <>
struct fmt::formatter<mpq_class> : ::flatsurf::GenericFormatter<mpq_class> {};

namespace flatsurf {

template <typename Surface>
AssertedLengths<Surface>::AssertedLengths(std::shared_ptr<const Vertical<FlatTriangulation<T>>> vertical, EdgeMap<std::optional<SaddleConnection<FlatTriangulation<T>>>>&& lengths) :
  lengths(vertical, std::move(lengths)),
  reference(this->lengths.forget()) {}

template <typename Surface>
void AssertedLengths<Surface>::push(intervalxt::Label label) {
  lengths.push(label);
  reference.push(label);
}

template <typename Surface>
void AssertedLengths<Surface>::pop() {
  lengths.pop();
  reference.pop();
}

template <typename Surface>
void AssertedLengths<Surface>::subtract(intervalxt::Label label) {
  lengths.subtract(label);
  reference.subtract(label);
  assertLengths();
}

template <typename Surface>
intervalxt::Label AssertedLengths<Surface>::subtractRepeated(intervalxt::Label label) {
  const auto ret = lengths.subtractRepeated(label);
  const auto sample = reference.subtractRepeated(label);
  ASSERT(ret == sample, "subtractRepeated() behaves differently from the reference implementation");
  assertLengths();
  return ret;
}

template <typename Surface>
std::vector<std::vector<mpq_class>> AssertedLengths<Surface>::coefficients(const std::vector<intervalxt::Label>& labels) const {
  const auto ret = lengths.coefficients(labels);
  const auto sample = reference.coefficients(labels);
  ASSERT(ret == sample, "coefficients() behaves differently from the reference implementation");
  return ret;
}

template <typename Surface>
int AssertedLengths<Surface>::cmp(intervalxt::Label label) const {
  const auto ret = lengths.cmp(label);
  const auto sample = reference.cmp(label);
  ASSERT(ret == sample, "cmp() behaves differently from the reference implementation");
  return ret;
}

template <typename Surface>
int AssertedLengths<Surface>::cmp(intervalxt::Label lhs, intervalxt::Label rhs) const {
  const auto ret = lengths.cmp(lhs, rhs);
  const auto sample = reference.cmp(lhs, rhs);
  ASSERT(ret == sample, "cmp() behaves differently from the reference implementation");
  return ret;
}

template <typename Surface>
typename Surface::Coordinate AssertedLengths<Surface>::get(intervalxt::Label label) const {
  const auto ret = lengths.get(label);
  const auto sample = reference.get(label);
  ASSERT(ret == sample, "get() behaves differently from the reference implementation");
  return ret;
}

template <typename Surface>
bool AssertedLengths<Surface>::similar(::intervalxt::Label a, ::intervalxt::Label b, const ::intervalxt::Lengths& other, ::intervalxt::Label aa, ::intervalxt::Label bb) const {
  return forget().similar(a, b, other, aa, bb);
}

template <typename Surface>
std::string AssertedLengths<Surface>::render(intervalxt::Label label) const {
  return lengths.render(label);
}

template <typename Surface>
::intervalxt::Lengths AssertedLengths<Surface>::forget() const {
  return lengths.forget();
}

template <typename Surface>
::intervalxt::Lengths AssertedLengths<Surface>::only(const std::unordered_set<::intervalxt::Label>& labels) const {
  return lengths.only(labels);
}

template <typename Surface>
void AssertedLengths<Surface>::assertLengths() const {
  lengths.lengths.apply([&](Edge e, const auto& value) {
    if (value) {
      const auto label = lengths.toLabel(e);
      ASSERT(lengths.get(label) == reference.get(label), "lengths do not match for edge " << e << "; length is " << lengths.get(label) << " but in the reference implementation it is " << reference.get(label));
    }
  });
}

template <typename Surface>
std::ostream& operator<<(std::ostream& os, const AssertedLengths<Surface>& lengths) { return os << lengths.lengths; }
}

// Instantiations of templates so implementations are generated for the linker
#include "util/instantiate.ipp"

LIBFLATSURF_INSTANTIATE_MANY_WRAPPED((LIBFLATSURF_INSTANTIATE_WITHOUT_IMPLEMENTATION), AssertedLengths, LIBFLATSURF_SURFACE_TYPES)
