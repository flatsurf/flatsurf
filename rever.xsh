######################################################################
#  This file is part of flatsurf.
#
#        Copyright (C) 2020 Julian Rüth
#
#  flatsurf is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or (at your
#  option) any later version.
#
#  flatsurf is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with flatsurf. If not, see <https://www.gnu.org/licenses/>.
#####################################################################

import sys

try:
  input("Are you sure you are on the master branch which is identical to origin/master and the only pending changes are a version bump in the configure.ac of the library? [ENTER]")
except KeyboardInterrupt:
  sys.exit(1)

sys.path.insert(0, 'tools/rever')

import autodist

$PROJECT = 'flatsurf'

$ACTIVITIES = [
    'version_bump',
    'changelog',
    'tag',
    'autodist',
    'push_tag',
    'ghrelease',
    'forge',
]

$VERSION_BUMP_PATTERNS = [
    ('configure.ac', r'AC_INIT', r'AC_INIT([flatsurf], [$VERSION], [julian.rueth@fsfe.org])'),
    ('libflatsurf/configure.ac', r'AC_INIT', r'AC_INIT([libflatsurf], [$VERSION], [julian.rueth@fsfe.org])'),
    ('pyflatsurf/configure.ac', r'AC_INIT', r'AC_INIT([pyflatsurf], [$VERSION], [julian.rueth@fsfe.org])'),
]

$CHANGELOG_FILENAME = 'ChangeLog'
$CHANGELOG_TEMPLATE = 'TEMPLATE.rst'
$CHANGELOG_NEWS = 'doc/news'
$CHANGELOG_CATEGORIES = ('Added', 'Changed', 'Deprecated', 'Removed', 'Fixed', 'Performance')
$PUSH_TAG_REMOTE = 'git@github.com:flatsurf/flatsurf.git'

$GITHUB_ORG = 'flatsurf'
$GITHUB_REPO = 'flatsurf'

$GHRELEASE_ASSETS = ['flatsurf-' + $VERSION + '.tar.gz']

$FORGE_FEEDSTOCK_ORG = "conda-forge"
