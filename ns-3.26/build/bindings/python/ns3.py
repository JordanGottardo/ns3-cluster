import warnings
warnings.warn("the ns3 module is a compatibility layer and should not be used in newly written code", DeprecationWarning, stacklevel=2)

from ns.applications import *
from ns.internet import *
from ns.wave import *
from ns.wifi import *
