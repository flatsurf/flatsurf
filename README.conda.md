About flatsurf
==============

Home: https://github.com/flatsurf/flatsurf

Package license: GPL2

Feedstock license: BSD 3-Clause

Summary: Alex Eskin's polygon program



Current build status
====================


<table>
    
  <tr>
    <td>Azure</td>
    <td>
      <details>
        <summary>
          <a href="https://dev.azure.com/flatsurf/conda/_build/latest?definitionId=&branchName=master">
            <img src="https://dev.azure.com/flatsurf/conda/_apis/build/status/flatsurf-feedstock?branchName=master">
          </a>
        </summary>
        <table>
          <thead><tr><th>Variant</th><th>Status</th></tr></thead>
          <tbody><tr>
              <td>linux</td>
              <td>
                <a href="https://dev.azure.com/flatsurf/conda/_build/latest?definitionId=&branchName=master">
                  <img src="https://dev.azure.com/flatsurf/conda/_apis/build/status/flatsurf-feedstock?branchName=master&jobName=linux&configuration=linux_" alt="variant">
                </a>
              </td>
            </tr>
          </tbody>
        </table>
      </details>
    </td>
  </tr>
  <tr>
    <td>OSX</td>
    <td>
      <img src="https://img.shields.io/badge/OSX-disabled-lightgrey.svg" alt="OSX disabled">
    </td>
  </tr>
  <tr>
    <td>Windows</td>
    <td>
      <img src="https://img.shields.io/badge/Windows-disabled-lightgrey.svg" alt="Windows disabled">
    </td>
  </tr>
![ppc64le disabled](https://img.shields.io/badge/ppc64le-disabled-lightgrey.svg)
</table>

Current release info
====================

| Name | Downloads | Version | Platforms |
| --- | --- | --- | --- |
| [![Conda Recipe](https://img.shields.io/badge/recipe-flatsurf-green.svg)](https://anaconda.org/flatsurf/flatsurf) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/flatsurf.svg)](https://anaconda.org/flatsurf/flatsurf) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/flatsurf.svg)](https://anaconda.org/flatsurf/flatsurf) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/flatsurf.svg)](https://anaconda.org/flatsurf/flatsurf) |
| [![Conda Recipe](https://img.shields.io/badge/recipe-libflatsurf-green.svg)](https://anaconda.org/flatsurf/libflatsurf) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/libflatsurf.svg)](https://anaconda.org/flatsurf/libflatsurf) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/libflatsurf.svg)](https://anaconda.org/flatsurf/libflatsurf) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/libflatsurf.svg)](https://anaconda.org/flatsurf/libflatsurf) |

Installing flatsurf
===================

Installing `flatsurf` from the `flatsurf` channel can be achieved by adding `flatsurf` to your channels with:

```
conda config --add channels flatsurf
```

Once the `flatsurf` channel has been enabled, `flatsurf, libflatsurf` can be installed with:

```
conda install flatsurf libflatsurf
```

It is possible to list all of the versions of `flatsurf` available on your platform with:

```
conda search flatsurf --channel flatsurf
```




Updating flatsurf-feedstock
===========================

If you would like to improve the flatsurf recipe or build a new
package version, please fork this repository and submit a PR. Upon submission,
your changes will be run on the appropriate platforms to give the reviewer an
opportunity to confirm that the changes result in a successful build. Once
merged, the recipe will be re-built and uploaded automatically to the
`flatsurf` channel, whereupon the built conda packages will be available for
everybody to install and use from the `flatsurf` channel.
Note that all branches in the conda-forge/flatsurf-feedstock are
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
* [@videlec](https://github.com/videlec/)

