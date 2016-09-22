/*
 * ParametricValue.hpp
 *
 *  A wrapper around the expression parser
 *
 *  Created on: Feb 8, 2009
 *      Author: jima
 *      Modified: sidch
 */

#ifndef __ParametricValue_hpp__
#define __ParametricValue_hpp__

#include "mathexpr.hpp"
#include "Algebra3.hpp"

/** Holds a value parametrized by time. */
class ParametricValue
{
  public:
    /** Get the default value, at time 0. */
    virtual double getValue() const = 0;

    /** Get the value at a particular time. */
    virtual double getValue(double time) const = 0;

    /** Check if the functor is in an ok state. */
    virtual bool good() const { return true; }

    /** Destructor. */
    virtual ~ParametricValue() {};

}; // class ParametricValue


/** Holds a constant value. */
class ConstValue : public ParametricValue
{
  private:
    double value_;  ///< The constant value.

  public:
    /** Constructor. */
    ConstValue(double value) : value_(value) {}

    /** Get the constant value. */
    double getValue() const { return value_; }

    /** Get the constant value. */
    double getValue(double time) const { return value_; }

}; // class ConstValue


/** Holds an expression and its result. */
class ExprValue : public ParametricValue
{
  private:
    mutable double time_;
    double dgr_;
    RVar timevar_;
    RVar dgrvar_;
    RVar * vararray_[2];
    ROperation op_;
    std::string expr_;

  public:
    /** Construct from a math expression. */
    ExprValue(const char * expr) : timevar_("t" , &time_), dgrvar_("dgr", &dgr_), expr_(expr)
    {
      vararray_[0] = &timevar_;
      vararray_[1] = &dgrvar_;
      op_ = ROperation ( expr_.c_str(), 2, vararray_ );
      time_ = 0;
      dgr_ = M_PI / 180.0;
    }

    /** Copy constructor. */
    ExprValue(const ExprValue & expr) : time_(expr.time_), timevar_(expr.timevar_), dgrvar_( "dgr", &dgr_ ), expr_(expr.expr_)
    {
      // Need special copy and copy assignment to prevent from having vararray_ point off to some old class
      // (which would make values not update properly after a copy!)

      vararray_[0] = &timevar_;
      vararray_[1] = &dgrvar_;
      op_ = ROperation(expr_.c_str(), 2, vararray_);
      dgr_ = M_PI / 180.0;
    }

    /** Destructor. */
    virtual ~ExprValue() {}

    /** Assignment operator. */
    ExprValue & operator=(const ExprValue & expr)
    {
      time_ = expr.time_;
      dgr_ = M_PI / 180.0;
      timevar_ = RVar(expr.timevar_);
      dgrvar_  = RVar("dgr", &dgr_);
      expr_ = expr.expr_;
      vararray_[0] = &timevar_;
      vararray_[1] = &dgrvar_;
      op_ = ROperation(expr_.c_str(), 2, vararray_);
      return *this;
    }

    /** Was the expression correctly parsed? */
    bool good() const
    {
      return !op_.HasError();
    }

    /** @inheritDoc */
    double getValue() const
    {
      time_ = 0; // set default time
      return op_.Val();
    }

    /** @inheritDoc */
    double getValue(double time) const
    {
      time_ = time;
      return op_.Val();
    }

}; // class ExprValue

#endif  // __ParametricValue_hpp__
