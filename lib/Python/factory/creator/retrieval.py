import numpy as np

from .base import Creator
from .. import param

from refractor import framework as rf

class StateVector(Creator):

    def __init__(self, *vargs, **kwargs):
        super().__init__(*vargs, **kwargs)

        self.register_to_receive(rf.StateVectorObserver)

        self.sv_observers = []

    def receive(self, rec_obj):

        if hasattr(rec_obj, "sub_state_identifier") and rec_obj.coefficient.value.shape[0] == 0:
            # Ignore SubStateVectorObservers that do not have any coefficients for the StateVector
            return
        elif rec_obj not in self.sv_observers:
            self.sv_observers.append(rec_obj)

    def create(self, **kwargs):
        sv = rf.StateVector()

        # Register observers into state vector and gather initial guess values
        ig_values = []
        for observer in self.sv_observers:
            sv.add_observer(observer)

            if hasattr(observer, "coefficient"):
                ig_values.append(observer.coefficient.value[np.nonzero(observer.used_flag_value)])

        ig = np.concatenate(ig_values)

        if ig.shape[0] != sv.observer_claimed_size:
            raise ValueError("The initial guess vector size %d does not match expected state vector size %d" % (ig.shape[0], sv.observer_claimed_size))
        
        sv.update_state(ig)

        return sv


