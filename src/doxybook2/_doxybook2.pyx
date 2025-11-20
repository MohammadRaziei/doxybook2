# distutils: language = c++
# cython: language_level=3

from libcpp.string cimport string
from libcpp.optional cimport optional

cdef extern from "DoxybookBinding.hpp" namespace "doxybook2":
    int generate(const string& input_dir,
                 const string& output_dir,
                 const optional[string]& config_path,
                 const optional[string]& templates_path,
                 bint json_only,
                 bint quiet,
                 bint debug_templates) except +


def build_docs(str input_dir,
               str output_dir,
               str config_path=None,
               str templates_path=None,
               bint json_only=False,
               bint quiet=False,
               bint debug_templates=False):
    """Generate documentation using the bundled Doxybook2 library."""

    cdef optional[string] cfg = optional[string]()
    cdef optional[string] tpl = optional[string]()
    cdef string cfg_value
    cdef string tpl_value

    if config_path is not None:
        cfg_value = config_path
        cfg = cfg_value
    if templates_path is not None:
        tpl_value = templates_path
        tpl = tpl_value

    status = generate(input_dir, output_dir, cfg, tpl, json_only, quiet, debug_templates)
    if status != 0:
        raise RuntimeError("Doxybook2 generation failed with status %d" % status)

