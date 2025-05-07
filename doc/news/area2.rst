**Added:**

* Added ``FlowComponent::area2()`` as a replacement for ``FlowComponent::area()``.
* Added ``Vector::area2()`` as a replacement for ``Vector::area()``.
* Added ``ContourComponent::area()`` as a replacement for ``ContourComponent::area()``.

**Changed:**

* <news item>

**Deprecated:**

* Deprecated ``FlowComponent::area()`` since it returns twice the area. Use ``area2()`` instead.
* Deprecated ``Vector::area()`` since it returns twice the area. Use ``area2()`` instead.
* Deprecated ``ContourComponent::area()`` since it strangely returns 6 times the area. Use ``area2()`` instead.

**Removed:**

* Removed ``ContourComponent::width()`` from the API since it had no implementation at all.

**Fixed:**

* <news item>

**Performance:**

* <news item>
