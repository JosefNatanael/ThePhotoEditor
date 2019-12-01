/**
 * @class AbstractImageFilterTransform
 * @author your name (you@domain.com)
 * @brief Abstract base class for image filter transforms.
 * @details All descendents needs to override getName() and applyFilter().
 */

#include "AbstractImageFilterTransform.h"

/**
 * @brief Construct a new Abstract Image Filter Transform:: Abstract Image Filter Transform object
 * 
 * @param parent Passed to QObject() constructor.
 */
AbstractImageFilterTransform::AbstractImageFilterTransform(QObject *parent) : QObject(parent)
{

}
