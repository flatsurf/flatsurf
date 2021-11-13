# sage -t tries to invoke pytest on our files which does not always work.
# We use this file to pretend that pytest is not installed, see https://trac.sagemath.org/ticket/31103#comment:49
raise ModuleNotFoundError
