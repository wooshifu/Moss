#!/usr/bin/env python3

import os
import sys

if __name__ == '__main__':
    here = os.path.dirname(os.path.abspath(__file__))

    kconfig = os.path.join(here, 'Kconfiglib')
    sys.path.insert(len(sys.path), kconfig)

    from Kconfiglib import menuconfig

    # noinspection PyProtectedMember
    menuconfig._main()
