#######################################################################
# Makefile for java test-suite
#######################################################################

LANGUAGE     = java
JAVA         = java
JAVAC        = javac
JAVAFLAGS    = -Xcheck:jni
SCRIPTSUFFIX = _runme.java
srcdir       = .
top_srcdir   = ../../../..
top_builddir = ../../../..

C_TEST_CASES = \
	java_lib_arrays \
	java_lib_various

CPP_TEST_CASES = \
	enum_thorough_proper \
	enum_thorough_simple \
	enum_thorough_typeunsafe \
	exception_partial_info \
	intermediary_classname \
	java_constants \
	java_director \
	java_director_assumeoverride \
	java_enums \
	java_lib_arrays_dimensionless \
	java_lib_various \
	java_jnitypes \
	java_pgcpp \
	java_pragmas \
	java_prepost \
	java_throws \
	java_typemaps_proxy \
	java_typemaps_typewrapper
#	li_boost_intrusive_ptr

include $(srcdir)/../common.mk

# Overridden variables here
JAVA_PACKAGE = $*
SWIGOPT += -package $(JAVA_PACKAGE)

# Custom tests - tests with additional commandline options
nspace.%: JAVA_PACKAGE = $*Package
nspace_extend.%: JAVA_PACKAGE = $*Package
director_nspace.%: JAVA_PACKAGE = $*Package

# Rules for the different types of tests
%.cpptest: 
	$(setup)
	+(cd $(JAVA_PACKAGE) && $(swig_and_compile_cpp))
	$(run_testcase)

%.ctest:
	$(setup)
	+(cd $(JAVA_PACKAGE) && $(swig_and_compile_c))
	$(run_testcase)

%.multicpptest: 
	$(setup)
	+(cd $(JAVA_PACKAGE) && $(swig_and_compile_multi_cpp))
	$(run_testcase)

# Makes a directory for the testcase if it does not exist
setup = \
	if [ -f $(srcdir)/$(SCRIPTPREFIX)$*$(SCRIPTSUFFIX) ]; then	  \
	  echo "$(ACTION)ing testcase $* (with run test) under $(LANGUAGE)" ; \
	else								  \
	  echo "$(ACTION)ing testcase $* under $(LANGUAGE)" ;		  \
	fi;								  \
	if [ ! -d $(JAVA_PACKAGE) ]; then						  \
	  mkdir $(JAVA_PACKAGE);							  \
	fi

# Compiles java files then runs the testcase. A testcase is only run if
# a file is found which has _runme.java appended after the testcase name.
# Note Java uses LD_LIBRARY_PATH under Unix, PATH under Cygwin/Windows, SHLIB_PATH on HPUX and DYLD_LIBRARY_PATH on Mac OS X.
run_testcase = \
	cd $(JAVA_PACKAGE) && $(COMPILETOOL) $(JAVAC) -classpath . `find . -name "*.java"` && cd .. && \
	if [ -f $(srcdir)/$(SCRIPTPREFIX)$*$(SCRIPTSUFFIX) ]; then \
	  $(COMPILETOOL) $(JAVAC) -classpath . -d . $(srcdir)/$(SCRIPTPREFIX)$*$(SCRIPTSUFFIX) && \
	  env LD_LIBRARY_PATH="$(JAVA_PACKAGE):$$LD_LIBRARY_PATH" PATH="$(JAVA_PACKAGE):$$PATH" SHLIB_PATH="$(JAVA_PACKAGE):$$SHLIB_PATH" DYLD_LIBRARY_PATH="$(JAVA_PACKAGE):$$DYLD_LIBRARY_PATH" $(RUNTOOL) $(JAVA) $(JAVAFLAGS) -classpath . $*_runme; \
	fi

# Clean: remove testcase directories
%.clean:
	@if [ -d $(JAVA_PACKAGE) ]; then \
	  rm -rf $(JAVA_PACKAGE); \
	fi

clean:
	@rm -f *.class hs_err*.log
