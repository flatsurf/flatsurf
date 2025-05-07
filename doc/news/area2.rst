**Added:**

* Added ``FlowComponent::area2()`` as a replacement for ``FlowComponent::area()``.
* Added ``Vector::area2()`` as a replacement for ``Vector::area()``.
* Added ``ContourComponent::area2()`` as a replacement for ``ContourComponent::area()``.
* Added ``FlatTriangulation::area2()`` as a replacement for ``FlatTriangulation::area()``.
* Added ``FlowComponent::heightRelative()`` and ``FlowComponent::widthRelative()`` as replacements for ``width()`` and ``height()``.

**Deprecated:**

* Deprecated ``FlowComponent::area()`` since it returns twice the area. Use ``area2()`` instead.
* Deprecated ``Vector::area()`` since it returns twice the area. Use ``area2()`` instead.
* Deprecated ``ContourComponent::area()`` since it strangely returns 6 times the area. Use ``area2()`` instead.
* Deprecated ``FlatTriangulation::area()`` since it returns twice the area. Use ``area2()`` instead.
* Deprecated ``FlatTriangulationCollapsed::area()`` since it is not clear what the area of a collapsed surface would be. Use ``uncollapsed().area2()`` instead.
* Deprecated ``Path::area()`` since it is not really meaningful unless the path is the perimeter of a flow component.
* Deprecated ``FlowComponent::width()`` since it returns a scaled width.
* Deprecated ``FlowComponent::height()`` since it returns a scaled height.

**Removed:**

* Removed ``ContourComponent::width()`` from the API since it had no implementation at all.
