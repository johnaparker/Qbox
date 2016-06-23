#ifndef GUARD_qbox_h
#define GUARD_qbox_h

/// qbox is the global namespace for Qbox
namespace qbox{}

//main
#include "qbox/field2.h"
#include "qbox/h5out.h"
#include "qbox/field.h"

//objects
#include "qbox/objects/cylinder.h"
#include "qbox/objects/medium.h"
#include "qbox/objects/rectangle.h"

//monitors
#include "qbox/monitors/surface_monitor.h"
#include "qbox/monitors/box_monitor.h"

//sources
#include "qbox/sources/continuous_point_source.h"
#include "qbox/sources/gaussian_point_source.h"
#include "qbox/sources/custom_point_source.h"
#include "qbox/sources/continuous_line_source.h"
#include "qbox/sources/gaussian_line_source.h"
#include "qbox/sources/custom_line_source.h"

#endif

