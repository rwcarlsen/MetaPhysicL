//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// MetaPhysicL - A metaprogramming library for physics calculations
//
// Copyright (C) 2013 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor,
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-
//
// $Id$
//
//--------------------------------------------------------------------------


#ifndef METAPHYSICL_DUALNUMBER_H
#define METAPHYSICL_DUALNUMBER_H

#include "metaphysicl/dualnumber_decl.h"

#include "metaphysicl/dualnumber_surrogate.h"
#include "metaphysicl/metaphysicl_common.h"

namespace MetaPhysicL {

template <typename T, typename D>
class NotADuckDualNumber;

// static member initialization
template <typename T, typename D, bool asd>
bool DualNumber<T,D,asd>::do_derivatives = true;

// Member definitions
template <typename T, typename D, bool asd>
inline
T&
DualNumber<T,D,asd>::value() { return _val; }

template <typename T, typename D, bool asd>
inline
const T&
DualNumber<T,D,asd>::value() const { return _val; }

template <typename T, typename D, bool asd>
inline
D&
DualNumber<T,D,asd>::derivatives() { return _deriv; }

template <typename T, typename D, bool asd>
inline
const D&
DualNumber<T,D,asd>::derivatives() const { return _deriv; }

template <typename T, typename D, bool asd>
inline
bool
DualNumber<T,D,asd>::boolean_test() const { return _val; }

template <typename T, typename D, bool asd>
inline
DualNumber<T,D,asd>
DualNumber<T,D,asd>::operator- () const { return DualNumber<T,D,asd>(-_val, -_deriv); }

template <typename T, typename D, bool asd>
inline
DualNumber<T,D,asd>
DualNumber<T,D,asd>::operator! () const { return DualNumber<T,D,asd>(!_val, !_deriv); }

template <typename T, typename D, bool asd>
template <typename T2, typename D2>
inline
DualNumber<T,D,asd> &
DualNumber<T,D,asd>::operator=(const DualNumber<T2,D2,asd> & dn)
{
  _label = dn._label;
  _val = dn.value();

  if (!allow_skipping_derivatives || do_derivatives)
    _deriv = dn.derivatives();

  return *this;
}

template <typename T, typename D, bool asd>
inline
DualNumber<T,D,asd> &
DualNumber<T,D,asd>::operator=(const DualNumber<T,D,asd> & dn)
{
  _label = dn._label;
  _val = dn.value();

  if (!allow_skipping_derivatives || do_derivatives)
    _deriv = dn.derivatives();

  return *this;
}

#ifdef METAPHYSICL_USE_STD_MOVE
template <typename T, typename D, bool asd>
inline
DualNumber<T,D,asd> &
DualNumber<T,D,asd>::operator=(DualNumber<T,D,asd> && dn)
{
  _label = std::move(dn._label);
  _val = std::move(dn.value());

  if (!allow_skipping_derivatives || do_derivatives)
    _deriv = std::move(dn.derivatives());

  return *this;
}
#endif // METAPHYSICL_USE_STD_MOVE

template <typename T, typename D, bool asd>
template <typename T2, typename D2>
inline
DualNumber<T,D,asd> &
DualNumber<T,D,asd>::operator=(const NotADuckDualNumber<T2,D2> & nd_dn)
{
  _label = nd_dn._label;
  _val = nd_dn.value();

  if (!allow_skipping_derivatives || do_derivatives)
    _deriv = nd_dn.derivatives();

  return *this;
}

template <typename T, typename D, bool asd>
inline
DualNumber<T,D,asd>::DualNumber(const DualNumber<T,D,asd> & dn) :
    _val(dn.value()), _label(dn._label)
{
  if (!allow_skipping_derivatives || do_derivatives)
    _deriv = dn.derivatives();
}

#ifdef METAPHYSICL_USE_STD_MOVE
template <typename T, typename D, bool asd>
inline
DualNumber<T,D,asd>::DualNumber(DualNumber<T,D,asd> && dn) :
    _val(std::move(dn.value())), _label(std::move(dn._label))
{
  if (!allow_skipping_derivatives || do_derivatives)
    _deriv = std::move(dn.derivatives());
}
#endif // METAPHYSICL_USE_STD_MOVE

template <typename T, typename D, bool asd>
template <typename T2, typename D2>
inline
DualNumber<T,D,asd>::DualNumber(const DualNumberSurrogate<T2,D2> & dns) :
    _val(dns.value()), _label(dns._label)
{
  if (!allow_skipping_derivatives || do_derivatives)
  {
    auto size = dns.derivatives().size();
    for (decltype(size) i = 0; i < size; ++i)
      _deriv[i] = *dns.derivatives()[i];
  }
}

template <typename T, typename D, bool asd>
template <typename T2, typename D2>
inline
DualNumber<T,D,asd> &
DualNumber<T,D,asd>::operator=(const DualNumberSurrogate<T2,D2> & dns)
{
  _label = dns._label;
  _val = dns.value();

  if (!allow_skipping_derivatives || do_derivatives)
  {
    auto size = dns.derivatives().size();
    for (decltype(size) i = 0; i < size; ++i)
      _deriv[i] = *dns.derivatives()[i];
  }
  return *this;
}

template <typename T, typename D, bool asd>
template <typename T2>
inline
DualNumber<T,D,asd> &
DualNumber<T,D,asd>::operator=(const T2 & scalar)
{
  _val = scalar;
  if (!allow_skipping_derivatives || do_derivatives)
    _deriv = 0;
  return *this;
}

//
// Member function definitions
//

// permit construction from another dual number if both value and derivative types are convertible
template <typename T, typename D, bool asd>
template <typename T2, typename D2>
inline
DualNumber<T,D,asd>::DualNumber(const DualNumber<T2, D2, asd> & val, typename std::enable_if<std::is_convertible<T2,T>::value && std::is_convertible<D2,D>::value, void*>::type) :
    _val  (DualNumberConstructor<T,D,asd>::value(val))
{
  if (!allow_skipping_derivatives || do_derivatives)
    _deriv = DualNumberConstructor<T,D,asd>::deriv(val);
}

// permit construction from any type, if it can be converted to this dual number's value type
template <typename T, typename D, bool asd>
template <typename T2>
inline
DualNumber<T,D,asd>::DualNumber(const T2& val, typename std::enable_if<std::is_convertible<T2,T>::value, void*>::type) :
    _val  (DualNumberConstructor<T,D,asd>::value(val))
{
  if (!allow_skipping_derivatives || do_derivatives)
    _deriv = DualNumberConstructor<T,D,asd>::deriv(val);
}

template <typename T, typename D, bool asd>
template <typename T2, typename D2>
inline
DualNumber<T,D,asd>::DualNumber(const T2& val,
                            const D2& deriv) :
  _val  (DualNumberConstructor<T,D,asd>::value(val,deriv))
{
  if (!allow_skipping_derivatives || do_derivatives)
    _deriv = DualNumberConstructor<T,D,asd>::deriv(val,deriv);
}

// Some helpers for reducing temporary creation and memset, memcpy calls

template <typename T,
          template <std::size_t, typename>
          class D,
          std::size_t N,
          typename DT,
          bool asd,
          typename std::enable_if<ScalarTraits<T>::value, int>::type = 0>
inline void
derivative_multiply_helper(DualNumber<T, D<N, DT>, asd> & out, const DualNumber<T, D<N, DT>, asd> & in)
{
  // do_derivatives is a static member so only need to check one object
  if (asd && !out.do_derivatives)
    return;

  auto & din = in.derivatives();
  auto & dout = out.derivatives();
  const auto vin = in.value();
  const auto vout = out.value();
  const auto n = dout.size();
  for (decltype(dout.size()) i = 0; i < n; i++)
    dout[i] = vin * dout[i] + vout * din[i];
}

template <typename T, typename D, bool asd>
inline void
derivative_multiply_helper(DualNumber<T, D, asd> & out, const DualNumber<T, D, asd> & in)
{
  // do_derivatives is a static member so only need to check one object
  if (asd && !out.do_derivatives)
    return;

  if (&in == &out)
    out.derivatives() = out.derivatives() * in.value() + out.value() * in.derivatives();
  else
  {
    out.derivatives() *= in.value();
    out.derivatives() += out.value() * in.derivatives();
  }
}

template <typename T, typename D, typename T2, typename D2, bool asd>
inline void
derivative_multiply_helper(DualNumber<T, D, asd> & out, const DualNumber<T2, D2, asd> & in)
{
  // Potentially different classes so need to check both
  if (asd && !(out.do_derivatives || in.do_derivatives))
    return;

  out.derivatives() *= in.value();
  out.derivatives() += out.value() * in.derivatives();
}

template <typename T,
          template <std::size_t, typename>
          class D,
          std::size_t N,
          typename DT,
          bool asd,
          typename std::enable_if<ScalarTraits<T>::value, int>::type = 0>
inline void
derivative_division_helper(DualNumber<T, D<N, DT>, asd> & out, const DualNumber<T, D<N, DT>, asd> & in)
{
  // do_derivatives is a static member so only need to check one object
  if (asd && !out.do_derivatives)
    return;
  auto & din = in.derivatives();
  auto & dout = out.derivatives();
  const auto vin = in.value();
  const auto vout = out.value();
  const auto n = dout.size();
  for (decltype(dout.size()) i = 0; i < n; i++)
    dout[i] = dout[i] / vin - din[i] * vout / (vin * vin);
}

template <typename T, typename D, bool asd>
inline void
derivative_division_helper(DualNumber<T, D, asd> & out, const DualNumber<T, D, asd> & in)
{
  // do_derivatives is a static member so only need to check one object
  if (asd && !out.do_derivatives)
    return;

  if (&in == &out)
    out.derivatives() =
      out.derivatives() / in.value() - out.value() / (in.value() * in.value()) * in.derivatives();
  else
  {
    out.derivatives() /= in.value();
    out.derivatives() -= out.value()/(in.value()*in.value()) * in.derivatives();
  }
}

template <typename T, typename D, typename T2, typename D2, bool asd>
inline void
derivative_division_helper(DualNumber<T, D, asd> & out, const DualNumber<T2, D2, asd> & in)
{
  // Potentially different classes so need to check both
  if (asd && !(out.do_derivatives || in.do_derivatives))
    return;
  out.derivatives() /= in.value();
  out.derivatives() -= out.value()/(in.value()*in.value()) * in.derivatives();
}

// FIXME: these operators currently do automatic type promotion when
// encountering DualNumbers of differing levels of recursion and
// differentiability.  But what we really want is automatic type
// *demotion*, to avoid pretending we have accurate derivatives which
// we don't have.  If we could do that right then some potential
// subtle run-time user errors would turn into compile-time user
// errors.

#define DualNumber_preop(opname, functorname, simplecalc, dualcalc) \
template <typename T, typename D, bool asd> \
template <typename T2> \
inline \
DualNumber<T,D,asd>& \
DualNumber<T,D,asd>::operator opname##= (const T2& in) \
{ \
  if (!allow_skipping_derivatives || do_derivatives) \
  { \
    simplecalc; \
  } \
  if (_label.size() > 0) \
    _label = "("#opname" " + show() + " " + std::to_string(in) + ")"; \
  this->value() opname##= in; \
  return *this; \
} \
 \
template <typename T, typename D, bool asd> \
template <typename T2, typename D2> \
inline \
DualNumber<T,D,asd>& \
DualNumber<T,D,asd>::operator opname##= (const DualNumber<T2,D2,asd>& in) \
{ \
  if (!allow_skipping_derivatives || do_derivatives) \
  { \
    dualcalc; \
  } \
  if (_label.size() + in._label.size() > 0) \
    _label = "("#opname" " + show() + " " + in.show() + ")"; \
  this->value() opname##= in.value(); \
  return *this; \
} \
 \
template <typename T, typename D, bool asd> \
template <typename T2, typename D2> \
inline \
DualNumber<T,D,asd> & \
DualNumber<T,D,asd>::operator opname##= (const NotADuckDualNumber<T2,D2>& in) \
{ \
  if (!allow_skipping_derivatives || do_derivatives) \
  { \
    dualcalc; \
  } \
  if (_label.size() + in._label.size() > 0) \
    _label = "("#opname" " + show() + " " + in.show() + ")"; \
  this->value() opname##= in.value(); \
  return *this; \
} \
 \
template <typename T, typename D, typename T2, typename D2, bool asd> \
inline \
typename functorname##Type<DualNumber<T,D,asd>,DualNumber<T2,D2,asd> >::supertype \
operator opname (const DualNumber<T,D,asd>& a, const DualNumber<T2,D2,asd>& b) \
{ \
  typedef typename \
    functorname##Type<DualNumber<T,D,asd>,DualNumber<T2,D2,asd> >::supertype \
    DS; \
  DS returnval = a; \
  returnval opname##= b; \
  if (a.label().size() + b.label().size() > 0) \
    returnval.label("("#opname" " + a.show() + " " + b.show() + ")"); \
  return returnval; \
} \
 \
template <typename T, typename T2, typename D, bool asd> \
inline \
typename functorname##Type<DualNumber<T2,D,asd>,T,true>::supertype \
operator opname (const T& a, const DualNumber<T2,D,asd>& b) \
{ \
  typedef typename \
    functorname##Type<DualNumber<T2,D,asd>,T,true>::supertype DS; \
  DS returnval = a; \
  returnval opname##= b; \
  if (b.label().size() > 0) \
    returnval.label("("#opname" " + std::to_string(a) + " " + b.show() + ")"); \
  return returnval; \
} \
 \
template <typename T, typename D, typename T2, bool asd> \
inline \
typename functorname##Type<DualNumber<T,D,asd>,T2,false>::supertype \
operator opname (const DualNumber<T,D,asd>& a, const T2& b) \
{ \
  typedef typename \
    functorname##Type<DualNumber<T,D,asd>,T2,false>::supertype DS; \
  DS returnval = a; \
  returnval opname##= b; \
  if (a.label().size() > 0) \
    returnval.label("("#opname" " + a.show() + " " + std::to_string(b) + ")"); \
  return returnval; \
}


// With C++11, define "move operations" where possible.  We should be
// more complete and define the move-from-b alternatives as well, but
// those would require additional support to correctly handle
// division, subtraction, or non-commutative addition/multiplication
// TODO: insert expression extraction cout/prints in the move operators and be sure to move the _label vals too!
#ifdef METAPHYSICL_USE_STD_MOVE
#define DualNumber_op(opname, functorname, simplecalc, dualcalc) \
        DualNumber_preop(opname, functorname, simplecalc, dualcalc) \
 \
template <typename T, typename D, typename T2, typename D2, bool asd> \
inline \
typename functorname##Type<DualNumber<T,D,asd>,DualNumber<T2,D2,asd> >::supertype \
operator opname (DualNumber<T,D,asd>&& a, const DualNumber<T2,D2,asd>& b) \
{ \
  typedef typename \
    functorname##Type<DualNumber<T,D,asd>,DualNumber<T2,D2,asd> >::supertype \
    DS; \
  DS returnval = std::move(a); \
  returnval opname##= b; \
  return returnval; \
} \
 \
template <typename T, typename D, typename T2, bool asd> \
inline \
typename functorname##Type<DualNumber<T,D,asd>,T2,false>::supertype \
operator opname (DualNumber<T,D,asd>&& a, const T2& b) \
{ \
  typedef typename \
    functorname##Type<DualNumber<T,D,asd>,T2,false>::supertype DS; \
  DS returnval = std::move(a); \
  returnval opname##= b; \
  return returnval; \
}

#else
#define DualNumber_op(opname, functorname, simplecalc, dualcalc) \
        DualNumber_preop(opname, functorname, simplecalc, dualcalc)
#endif

DualNumber_op(+, Plus, , this->derivatives() += in.derivatives())

DualNumber_op(-, Minus, , this->derivatives() -= in.derivatives())

DualNumber_op(*,
              Multiplies,
              this->derivatives() *= in,
              derivative_multiply_helper(*this, in))

DualNumber_op(/,
              Divides,
              this->derivatives() /= in,
              derivative_division_helper(*this, in))


#define DualNumber_compare(opname)                          \
template <typename T, typename D, typename T2, typename D2, bool asd> \
inline \
bool \
operator opname  (const DualNumber<T,D,asd>& a, const DualNumber<T2,D2,asd>& b) \
{ \
  return (a.value() opname b.value()); \
} \
 \
template <typename T, typename T2, typename D2, bool asd> \
inline \
typename boostcopy::enable_if_class< \
  typename CompareTypes<DualNumber<T2,D2,asd>,T>::supertype, \
  bool \
>::type \
operator opname  (const T& a, const DualNumber<T2,D2,asd>& b) \
{ \
  return (a opname b.value()); \
} \
 \
template <typename T, typename T2, typename D, bool asd> \
inline \
typename boostcopy::enable_if_class< \
  typename CompareTypes<DualNumber<T,D,asd>,T2>::supertype, \
  bool \
>::type \
operator opname  (const DualNumber<T,D,asd>& a, const T2& b) \
{ \
  return (a.value() opname b); \
}

DualNumber_compare(>)
DualNumber_compare(>=)
DualNumber_compare(<)
DualNumber_compare(<=)
DualNumber_compare(==)
DualNumber_compare(!=)
DualNumber_compare(&&)
DualNumber_compare(||)

template <typename T, typename D, bool asd>
inline std::ostream &
operator<<(std::ostream & output, const DualNumber<T, D, asd> & a)
{
  return output << '(' << a.value() << ',' << a.derivatives() << ')';
}


template <typename T, typename D, bool asd>
inline
D gradient(const DualNumber<T, D, asd>& a)
{
  return a.derivatives();
}
} // namespace MetaPhysicL


namespace std {

using MetaPhysicL::DualNumber;
using MetaPhysicL::CompareTypes;

template <typename T, typename D, bool asd>
inline bool isnan (const DualNumber<T,D,asd> & a)
{
  using std::isnan;
  return isnan(a.value());
}

template <typename T, typename D, bool asd>
inline bool isinf (const DualNumber<T,D,asd> & a)
{
  using std::isinf;
  return isinf(a.value());
}


#if METAPHYSICL_USE_STD_MOVE
#define DualNumber_std_unary(funcname, derivative, precalc) \
template <typename T, typename D, bool asd> \
inline \
DualNumber<T,D,asd> funcname (const DualNumber<T,D,asd> & in) \
{ \
  DualNumber<T,D,asd> returnval = in; \
  T funcval = std::funcname(in.value()); \
  if (!asd || in.do_derivatives) \
  { \
    precalc; \
    returnval.derivatives() *= derivative; \
  } \
  returnval.value() = funcval; \
  return returnval; \
} \
 \
template <typename T, typename D, bool asd> \
inline \
DualNumber<T,D,asd> funcname (DualNumber<T,D,asd> && in) \
{ \
  T funcval = std::funcname(in.value()); \
  if (!asd || in.do_derivatives) \
  { \
    precalc; \
    in.derivatives() *= derivative; \
  } \
  in.value() = funcval; \
  return std::move(in); \
}

#define DualNumber_equiv_unary(funcname, equivalent) \
template <typename T, typename D, bool asd> \
inline \
DualNumber<T,D,asd> funcname (const DualNumber<T,D,asd> & in) \
{ \
  return std::equivalent(in); \
} \
 \
template <typename T, typename D, bool asd> \
inline \
DualNumber<T,D,asd> funcname (DualNumber<T,D,asd> && in) \
{ \
  return std::equivalent(in); \
}

#else

#define DualNumber_std_unary(funcname, derivative, precalc) \
template <typename T, typename D, bool asd> \
inline \
DualNumber<T,D,asd> funcname (DualNumber<T,D,asd> in) \
{ \
  T funcval = std::funcname(in.value()); \
  if (!asd || in.do_derivatives) \
  { \
    precalc; \
    in.derivatives() *= derivative; \
  } \
  in.value() = funcval; \
  return std::move(in); \
}

#define DualNumber_equiv_unary(funcname, equivalent) \
template <typename T, typename D, bool asd> \
inline \
DualNumber<T,D,asd> funcname (DualNumber<T,D,asd> in) \
{ \
  return std::equivalent(in); \
}

#endif

#define DualNumber_equivfl_unary(funcname) \
DualNumber_equiv_unary(funcname##f, funcname) \
DualNumber_equiv_unary(funcname##l, funcname)


DualNumber_std_unary(sqrt, 1 / (2 * funcval),)
DualNumber_std_unary(exp, funcval,)
DualNumber_std_unary(log, 1 / in.value(),)
DualNumber_std_unary(log10, 1 / in.value() * (1/std::log(T(10.))),)
DualNumber_std_unary(sin, std::cos(in.value()),)
DualNumber_std_unary(cos, -std::sin(in.value()),)
DualNumber_std_unary(tan, sec_in * sec_in, T sec_in = 1 / std::cos(in.value()))
DualNumber_std_unary(asin, 1 / std::sqrt(1 - in.value()*in.value()),)
DualNumber_std_unary(acos, -1 / std::sqrt(1 - in.value()*in.value()),)
DualNumber_std_unary(atan, 1 / (1 + in.value()*in.value()),)
DualNumber_std_unary(sinh, std::cosh(in.value()),)
DualNumber_std_unary(cosh, std::sinh(in.value()),)
DualNumber_std_unary(tanh, sech_in * sech_in, T sech_in = 1 / std::cosh(in.value()))
DualNumber_std_unary(abs, (in.value() > 0) - (in.value() < 0),) // std < and > return 0 or 1
DualNumber_equiv_unary(fabs, abs)
DualNumber_std_unary(norm, 2. * in.value(),)
DualNumber_std_unary(ceil, 0,)
DualNumber_std_unary(floor, 0,)

#if __cplusplus >= 201103L
DualNumber_equiv_unary(llabs, abs)
DualNumber_equiv_unary(imaxabs, abs)
DualNumber_equivfl_unary(fabs)
DualNumber_equivfl_unary(exp)
DualNumber_std_unary(exp2, std::log(T(2))*funcval,)
DualNumber_equivfl_unary(exp2)
DualNumber_std_unary(expm1, std::exp(in.value()),)
DualNumber_equivfl_unary(expm1)
DualNumber_equivfl_unary(log)
DualNumber_equivfl_unary(log10)
DualNumber_std_unary(log2, 1 / in.value() * (1/std::log(T(2))),)
DualNumber_equivfl_unary(log2)
DualNumber_std_unary(log1p, 1 / (in.value() + 1),)
DualNumber_equivfl_unary(log1p)
DualNumber_equivfl_unary(sqrt)
DualNumber_std_unary(cbrt, 1 / (3 * funcval * funcval),)
DualNumber_equivfl_unary(cbrt)
DualNumber_equivfl_unary(sin)
DualNumber_equivfl_unary(cos)
DualNumber_equivfl_unary(tan)
DualNumber_equivfl_unary(asin)
DualNumber_equivfl_unary(acos)
DualNumber_equivfl_unary(atan)
DualNumber_equivfl_unary(sinh)
DualNumber_equivfl_unary(cosh)
DualNumber_equivfl_unary(tanh)
DualNumber_std_unary(asinh, 1 / sqrt(1 + in.value()*in.value()),)
DualNumber_equivfl_unary(asinh)
DualNumber_std_unary(acosh, 1 / sqrt(in.value()*in.value() - 1),)
DualNumber_equivfl_unary(acosh)
DualNumber_std_unary(atanh, 1 / (1 - in.value()*in.value()),)
DualNumber_equivfl_unary(atanh)
// 2/sqrt(pi) = 1/sqrt(atan(1.0))
DualNumber_std_unary(erf, 1/sqrt(atan(T(1)))*exp(-in.value()*in.value()),)
DualNumber_equivfl_unary(erf)
DualNumber_std_unary(erfc, -1/sqrt(atan(T(1)))*exp(-in.value()*in.value()),)
DualNumber_equivfl_unary(erfc)
// FIXME: how do we differentiate tgamma and lgamma without boost?
DualNumber_equivfl_unary(ceil)
DualNumber_equivfl_unary(floor)
DualNumber_std_unary(trunc, 0,)
DualNumber_equivfl_unary(trunc)
DualNumber_std_unary(round, 0,)
DualNumber_equivfl_unary(round)
DualNumber_std_unary(nearbyint, 0,)
DualNumber_equivfl_unary(nearbyint)
DualNumber_std_unary(rint, 0,)
DualNumber_equivfl_unary(rint)
#endif // __cplusplus >= 201103L

#define DualNumber_complex_std_unary_real(funcname) \
template <typename T, typename D, bool asd> \
inline DualNumber<T, typename D::template rebind<T>::other, asd> \
funcname(const DualNumber<std::complex<T>, D, asd> & in) \
{ \
  return {std::funcname(in.value()), std::numeric_limits<double>::quiet_NaN()}; \
} \
template <typename T, bool asd> \
inline DualNumber<T,T,asd> \
funcname(const DualNumber<std::complex<T>,std::complex<T>,asd> & in)    \
{ \
  return {std::funcname(in.value()), std::numeric_limits<double>::quiet_NaN()}; \
}

DualNumber_complex_std_unary_real(real)
DualNumber_complex_std_unary_real(imag)
DualNumber_complex_std_unary_real(norm)
DualNumber_complex_std_unary_real(abs)

#define DualNumber_complex_std_unary_complex_pre(funcname) \
template <typename T, typename D, bool asd> \
inline DualNumber<std::complex<T>, D, asd> \
funcname(const DualNumber<std::complex<T>, D, asd> & in) \
{ \
  return {std::funcname(in.value()), std::complex<T>{std::numeric_limits<double>::quiet_NaN(), \
                                                     std::numeric_limits<double>::quiet_NaN()}}; \
} \
template <typename T, bool asd> \
inline DualNumber<std::complex<T>,std::complex<T>,asd> \
funcname(const DualNumber<std::complex<T>,std::complex<T>,asd> & in) \
{ \
  return {std::funcname(in.value()), std::complex<T>{std::numeric_limits<double>::quiet_NaN(), \
                                                     std::numeric_limits<double>::quiet_NaN()}}; \
}

#if METAPHYSICL_USE_STD_MOVE
#define DualNumber_complex_std_unary_complex(funcname) \
DualNumber_complex_std_unary_complex_pre(funcname) \
template <typename T, typename D, bool asd> \
inline DualNumber<std::complex<T>, D, asd> \
funcname(DualNumber<std::complex<T>, D, asd> && in) \
{ \
  in.value() = std::funcname(in.value()); \
  in.derivatives() = std::complex<T>(std::numeric_limits<double>::quiet_NaN(), \
                                     std::numeric_limits<double>::quiet_NaN()); \
  return in; \
} \
template <typename T, bool asd> \
inline DualNumber<std::complex<T>,std::complex<T>,asd> \
funcname(DualNumber<std::complex<T>,std::complex<T>,asd> && in) \
{ \
  in.value() = std::funcname(in.value()); \
  in.derivatives() = std::complex<T>(std::numeric_limits<double>::quiet_NaN(), \
                                     std::numeric_limits<double>::quiet_NaN()); \
  return in; \
}
#else
#define DualNumber_complex_std_unary_complex(funcname) \
DualNumber_complex_std_unary_complex_pre(funcname)
#endif

DualNumber_complex_std_unary_complex(conj)

#define DualNumber_std_binary(funcname, derivative, rightderiv, leftderiv) \
template <typename T, typename D, typename T2, typename D2, bool asd> \
inline \
typename CompareTypes<DualNumber<T,D,asd>,DualNumber<T2,D2,asd> >::supertype \
funcname (const DualNumber<T,D,asd>& a, const DualNumber<T2,D2,asd>& b) \
{ \
  typedef typename CompareTypes<T,T2>::supertype TS; \
  typedef typename CompareTypes<DualNumber<T,D,asd>,DualNumber<T2,D2,asd> >::supertype type; \
 \
  TS funcval = std::funcname(a.value(), b.value()); \
  if (asd && !(a.do_derivatives || b.do_derivatives)) \
    return type(funcval, 0); \
  else \
    return type(funcval, derivative); \
} \
 \
template <typename T, typename D, bool asd> \
inline \
DualNumber<T,D,asd> \
funcname (const DualNumber<T,D,asd>& a, const DualNumber<T,D,asd>& b) \
{ \
  T funcval = std::funcname(a.value(), b.value()); \
  if (asd && !a.do_derivatives) \
    return DualNumber<T,D,asd>(funcval, 0); \
  else \
    return DualNumber<T,D,asd>(funcval, derivative); \
} \
 \
template <typename T, typename T2, typename D, bool asd> \
inline \
typename CompareTypes<DualNumber<T2,D,asd>,T,true>::supertype \
funcname (const T& a, const DualNumber<T2,D,asd>& b) \
{ \
  typedef typename CompareTypes<T,T2>::supertype TS; \
  TS funcval = std::funcname(a, b.value()); \
  typedef typename CompareTypes<DualNumber<T2,D,asd>,T,true>::supertype type; \
  if (asd && !b.do_derivatives) \
    return type(funcval, 0); \
  else \
    return type(funcval, rightderiv); \
} \
 \
template <typename T, typename T2, typename D, bool asd> \
inline \
typename CompareTypes<DualNumber<T,D,asd>,T2>::supertype \
funcname (const DualNumber<T,D,asd>& a, const T2& b) \
{ \
  typedef typename CompareTypes<T,T2>::supertype TS; \
  TS funcval = std::funcname(a.value(), b); \
  typedef typename CompareTypes<DualNumber<T,D,asd>,T2>::supertype type; \
  if (asd && !a.do_derivatives) \
    return type(funcval, 0); \
  else \
    return type(funcval, leftderiv); \
}

#define DualNumber_equiv_binary(funcname, equivalent) \
template <typename T, typename D, typename T2, typename D2, bool asd> \
inline \
typename CompareTypes<DualNumber<T,D,asd>,DualNumber<T2,D2,asd> >::supertype \
funcname (const DualNumber<T,D,asd>& a, const DualNumber<T2,D2,asd>& b) \
{ \
  return std::equivalent(a,b); \
} \
 \
template <typename T, typename D, bool asd> \
inline \
DualNumber<T,D,asd> \
funcname (const DualNumber<T,D,asd>& a, const DualNumber<T,D,asd>& b) \
{ \
  return std::equivalent(a,b); \
} \
 \
template <typename T, typename T2, typename D, bool asd> \
inline \
typename CompareTypes<DualNumber<T2,D,asd>,T,true>::supertype \
funcname (const T& a, const DualNumber<T2,D,asd>& b) \
{ \
  return std::equivalent(a,b); \
} \
 \
template <typename T, typename T2, typename D, bool asd> \
inline \
typename CompareTypes<DualNumber<T,D,asd>,T2>::supertype \
funcname (const DualNumber<T,D,asd>& a, const T2& b) \
{ \
  return std::equivalent(a,b); \
}

#define DualNumber_equivfl_binary(funcname) \
DualNumber_equiv_binary(funcname##f, funcname) \
DualNumber_equiv_binary(funcname##l, funcname)

// C++ turns 0u - 1 into max of signed int, not -1.  We do not want to
// do that when we're evaluating pow() derivatives.  But we also don't
// want to wastefully promote to FP.
//
// This fixes the case where we evaluate pow(DualNumber, unsigned
// int).  If anyone is using something like NumberArray of unsigned
// int it won't work; hopefully anybody with that level of
// sophistication can be trusted to diagnose the problem and switch to
// signed int.

inline int dn_val_minus_one(unsigned int e) { return int(e)-1; }
inline int dn_val_minus_one(std::size_t e) { return int(e)-1; }

template <typename T>
inline T dn_val_minus_one(const T & e) { return e-1; }

// if_else is necessary here to handle cases where a is non-positive
// but b is 0; we should have a contribution of 0 from those, not NaN.
// That doesn't avoid FP exceptions, though, not when b is a
// DualNumber ... not sure how to fix that.
DualNumber_std_binary(pow,
  std::pow(a.value(), dn_val_minus_one(b.value())) * (b.value() * a.derivatives() +
  MetaPhysicL::if_else(b.derivatives(), b.derivatives() * std::log(a.value()) * a.value(), b.derivatives())),
  std::pow(a, b.value()) *
  MetaPhysicL::if_else(b.derivatives(), (b.derivatives() * std::log(TS(a))), b.derivatives()),
  std::pow(a.value(), dn_val_minus_one(b)) * (b * a.derivatives())
  )
DualNumber_std_binary(atan2,
  (b.value() * a.derivatives() - a.value() * b.derivatives()) /
  (b.value() * b.value() + a.value() * a.value()),
  (-a * b.derivatives()) /
  (b.value() * b.value() + a * a),
  (b * a.derivatives()) /
  (b * b + a.value() * a.value()))
DualNumber_std_binary(max,
  (a.value() > b.value()) ? a.derivatives() : b.derivatives(),
  (a > b.value()) ? 0 : b.derivatives(),
  (a.value() > b) ? a.derivatives() : 0)
DualNumber_std_binary(min,
  (a.value() > b.value()) ? b.derivatives() : a.derivatives(),
  (a > b.value()) ? b.derivatives() : 0,
  (a.value() > b) ? 0 : a.derivatives())
DualNumber_std_binary(fmod, a.derivatives(), 0, a.derivatives())

#if __cplusplus >= 201103L
DualNumber_equivfl_binary(pow)
DualNumber_equivfl_binary(fmod)
DualNumber_std_binary(remainder, a.derivatives(), 0, a.derivatives())
DualNumber_equivfl_binary(remainder)
DualNumber_equiv_binary(fmax, max)
DualNumber_equivfl_binary(fmax)
DualNumber_equiv_binary(fmin, min)
DualNumber_equivfl_binary(fmin)
DualNumber_std_binary(fdim, if_else(a.value() > b.value(),
                                    a.derivatives() - b.derivatives(), 0),
                            if_else(a.value() > b, a.derivatives(), 0),
                            if_else(a > b.value(), -b.derivatives(), 0))
DualNumber_equivfl_binary(fdim)
DualNumber_std_binary(hypot, (a.value()*a.derivatives() +
                              b.value()*b.derivatives()) /
                              hypot(a.value(), b.value()),
                             (a.value()*a.derivatives()) /
                              hypot(a.value(), b),
                             (b.value()*b.derivatives()) /
                              hypot(a, b.value()))
DualNumber_equivfl_binary(hypot)
DualNumber_equivfl_binary(atan2)
#endif // __cplusplus >= 201103L

} // namespace std


#endif // METAPHYSICL_DUALNUMBER_H
