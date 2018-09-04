import os
from distutils.command.build import build as _build
from distutils import sysconfig


class build(_build):
    user_options = _build.user_options + [ ('cross-compile', 'x', 'set things up for a cross compile')]

    boolean_options = _build.boolean_options + ['cross-compile']

    def initialize_options(self):
        _build.initialize_options(self)
        self.cross_compile = 0

    def finalize_options(self):
        if self.cross_compile and os.environ.has_key('PYTHONXCPREFIX'):
            prefix = os.environ['PYTHONXCPREFIX']
            sysconfig.get_python_lib = get_python_lib
            sysconfig.PREFIX = prefix
            sysconfig.EXEC_PREFIX = prefix
            # reinitialize variables
            sysconfig._config_vars = None
            sysconfig.get_config_var("LDSHARED")

        _build.finalize_options(self)

    #def run(self):

    #    dutilbuild.build.run(self)


_get_python_lib = sysconfig.get_python_lib
def get_python_lib(plat_specific=0, standard_lib=0, prefix=None):
    if os.environ.has_key('PYTHONXCPREFIX'):
        print "Setting prefix"
        prefix = os.environ['PYTHONXCPREFIX']

    return _get_python_lib(plat_specific, standard_lib, prefix)

    
def customize_compiler(compiler):

    """Do any platform-specific customization of a CCompiler instance.

    Mainly needed on Unix, so we can plug in the information that
    varies across Unices and is stored in Python's Makefile.
    """
    if compiler.compiler_type == "unix":
        (cc, cxx, opt, cflags, ccshared, ldshared, so_ext) = \
            sysconfig.get_config_vars('CC', 'CXX', 'OPT', 'CFLAGS',
                            'CCSHARED', 'LDSHARED', 'SO')

        if os.environ.has_key('CC'):
            cc = os.environ['CC']
        if os.environ.has_key('CXX'):
            cxx = os.environ['CXX']
        if os.environ.has_key('LDSHARED'):
            ldshared = os.environ['LDSHARED']
        if os.environ.has_key('CPP'):
            cpp = os.environ['CPP']
        else:
            cpp = cc + " -E"           # not always
        if os.environ.has_key('LDFLAGS'):
            ldshared = ldshared + ' ' + os.environ['LDFLAGS']
        if os.environ.has_key('OPT'):
            opt = os.environ['OPT']
        if os.environ.has_key('CFLAGS'):
            cflags = opt + ' ' + os.environ['CFLAGS']
            ldshared = ldshared + ' ' + os.environ['CFLAGS']
        if os.environ.has_key('CPPFLAGS'):
            cpp = cpp + ' ' + os.environ['CPPFLAGS']
            cflags = cflags + ' ' + os.environ['CPPFLAGS']
            ldshared = ldshared + ' ' + os.environ['CPPFLAGS']

        cc_cmd = cc + ' ' + cflags
        compiler.set_executables(
            preprocessor=cpp,
            compiler=cc_cmd,
            compiler_so=cc_cmd + ' ' + ccshared,
            compiler_cxx=cxx,
            linker_so=ldshared,
            linker_exe=cc)

        compiler.shared_lib_extension = so_ext
