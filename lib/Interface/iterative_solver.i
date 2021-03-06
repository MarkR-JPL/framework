%include "common.i"
%{
#include "iterative_solver.h"
%}

%base_import(observer)

%import "observer.i"

%fp_shared_ptr(FullPhysics::IterativeSolver);

%fp_shared_ptr(FullPhysics::Observer<FullPhysics::IterativeSolver>);
%fp_shared_ptr(FullPhysics::Observable<FullPhysics::IterativeSolver>);
%template(ObserverIterativeSolver) FullPhysics::Observer<FullPhysics::IterativeSolver>;
%template(ObservableIterativeSolver) FullPhysics::Observable<FullPhysics::IterativeSolver>;

namespace FullPhysics {
class IterativeSolver : public Observable<IterativeSolver> {
public:
  enum status_t {SUCCESS, CONTINUE, STALLED, ERROR, UNTRIED};
  IterativeSolver(int max_cost_function_calls, bool vrbs);
  virtual ~IterativeSolver();
  virtual void add_observer(Observer<IterativeSolver>& Obs);
  virtual void remove_observer(Observer<IterativeSolver>& Obs);
  %python_attribute(num_accepted_steps, int)
  %python_attribute(accepted_points, std::vector< blitz::Array<double, 1> >)
  %python_attribute(cost_at_accepted_points, std::vector<double>)
  virtual void solve() = 0;
  %python_attribute(status, status_t)
  %python_attribute(status_str, std::string)
  std::string print_to_string() const;

  %extend {

        // Support shared pointers from other languages for add/remove observer
        void add_observer(const boost::shared_ptr<Observer<IterativeSolver> >& Obs) {
            $self->add_observer(*Obs);
        }

        void remove_observer(const boost::shared_ptr<Observer<IterativeSolver> >& Obs) {
            $self->remove_observer(*Obs);
        }

    }

};
}
