#!/usr/bin/env python
# -*- coding:utf-8 -*-

import random
print random
print random.randint(23, 89) # ==> Return random integer in range [23, 89], including both end points.
print random.choice(range(12, 24)) # ==> Choose a random element from a non-empty sequence.
print random.random() # ==> x in the interval [0, 1), eg. 0.19239290559
