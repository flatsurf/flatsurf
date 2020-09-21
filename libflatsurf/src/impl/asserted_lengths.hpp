#ifndef LIBFLATSURF_IMPL_ASSERTED_LENGTHS_HPP
#define LIBFLATSURF_IMPL_ASSERTED_LENGTHS_HPP

#include <intervalxt/label.hpp>
#include <intervalxt/lengths.hpp>

#include "../../flatsurf/flat_triangulation.hpp"
#include "lengths.hpp"
#include "flow_decomposition_state.hpp"

namespace flatsurf {

template <typename Surface>
class AssertedLengths {
  using T = typename Surface::Coordinate;

 public:
  AssertedLengths(std::shared_ptr<const Vertical<FlatTriangulation<T>>>, EdgeMap<std::optional<SaddleConnection<FlatTriangulation<T>>>>&&);

  void push(intervalxt::Label);
  void pop();
  void subtract(intervalxt::Label);
  intervalxt::Label subtractRepeated(intervalxt::Label);
  std::vector<std::vector<mpq_class>> coefficients(const std::vector<intervalxt::Label>&) const;
  int cmp(intervalxt::Label) const;
  int cmp(intervalxt::Label, intervalxt::Label) const;
  T get(intervalxt::Label) const;
  std::string render(intervalxt::Label) const;
  ::intervalxt::Lengths forget() const;
  ::intervalxt::Lengths only(const std::unordered_set<::intervalxt::Label>&) const;
  bool similar(::intervalxt::Label, ::intervalxt::Label, const ::intervalxt::Lengths&, ::intervalxt::Label, ::intervalxt::Label) const;

  // This is a hack, see https://github.com/flatsurf/flatsurf/issues/152.
  // Maybe the entire flatsurf::IntervalExchangeTransformation is a bit unfortunate actually.
  void registerDecomposition(std::shared_ptr<FlowDecompositionState<FlatTriangulation<T>>>);

  template <typename S>
  friend std::ostream& operator<<(std::ostream&, const AssertedLengths<S>&);

 private:
  Lengths<Surface> lengths;
  ::intervalxt::Lengths reference;

  void assertLengths() const;

  friend ImplementationOf<IntervalExchangeTransformation<Surface>>;
};

}

#endif
