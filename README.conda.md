<!--
# -*- mode: jinja -*-
-->

About polygon
=============

Home: https://github.com/polygon-tbd/polygon

Package license: GPL2

Feedstock license: BSD 3-Clause

Summary: Alex Eskin's polygon program



Current build status
====================

[![Linux](https://img.shields.io/circleci/project/github/conda-forge/polygon-feedstock/master.svg?label=Linux)](https://circleci.com/gh/conda-forge/polygon-feedstock)
[![OSX](https://img.shields.io/travis/conda-forge/polygon-feedstock/master.svg?label=macOS)](https://travis-ci.org/conda-forge/polygon-feedstock)
![Windows disabled](https://img.shields.io/badge/Windows-disabled-lightgrey.svg)

Current release info
====================

| Name | Downloads | Version | Platforms |
| --- | --- | --- | --- |
| [![Conda Recipe](https://img.shields.io/badge/recipe-polygon-green.svg)](https://anaconda.org/saraedum/polygon) | [![Conda Downloads](https://img.shields.io/conda/dn/saraedum/polygon.svg)](https://anaconda.org/saraedum/polygon) | [![Conda Version](https://img.shields.io/conda/vn/saraedum/polygon.svg)](https://anaconda.org/saraedum/polygon) | [![Conda Platforms](https://img.shields.io/conda/pn/saraedum/polygon.svg)](https://anaconda.org/saraedum/polygon) |

Installing polygon
==================

Installing `polygon` from the `saraedum` channel can be achieved by adding `saraedum` to your channels with:

```
conda config --add channels saraedum
```

Once the `saraedum` channel has been enabled, `polygon` can be installed with:

```
conda install polygon
```

It is possible to list all of the versions of `polygon` available on your platform with:

```
conda search polygon --channel saraedum
```




Updating polygon-feedstock
==========================

If you would like to improve the polygon recipe or build a new
package version, please fork this repository and submit a PR. Upon submission,
your changes will be run on the appropriate platforms to give the reviewer an
opportunity to confirm that the changes result in a successful build. Once
merged, the recipe will be re-built and uploaded automatically to the
`saraedum` channel, whereupon the built conda packages will be available for
everybody to install and use from the `saraedum` channel.
Note that all branches in the conda-forge/polygon-feedstock are
immediately built and any created packages are uploaded, so PRs should be based
on branches in forks and branches in the main repository should only be used to
build distinct package versions.

In order to produce a uniquely identifiable distribution:
 * If the version of a package **is not** being increased, please add or increase
   the [``build/number``](https://conda.io/docs/user-guide/tasks/build-packages/define-metadata.html#build-number-and-string).
 * If the version of a package **is** being increased, please remember to return
   the [``build/number``](https://conda.io/docs/user-guide/tasks/build-packages/define-metadata.html#build-number-and-string)
   back to 0.

Feedstock Maintainers
=====================

* [@saraedum](https://github.com/saraedum/)

