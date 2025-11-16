#include <libastfri-text/inc/TextLibManager.hpp>

#include <libastfri-text/inc/code/JavaCodeVisitor.hpp>

using namespace astfri::text;

TextLibManager& TextLibManager::get_instance()
{
    static TextLibManager manager;
    return manager;
}

TextLibManager::TextLibManager() :
    visitor_(&JavaCodeVisitor::get_instance())
{
}