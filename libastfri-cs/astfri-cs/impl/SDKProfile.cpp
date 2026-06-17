#include <astfri-cs/SDKProfile.hpp>

#include <stdexcept>


namespace astfri::csharp {


std::string_view sdk_profile_to_string(SDKProfile s) {
    switch (s) {
        case SDKProfile::Core:
            return "Core";
        case SDKProfile::None:
            return "None";
        case SDKProfile::Web:
            return "Web";
        case SDKProfile::WinForms:
            return "WinForms";
        case SDKProfile::Worker:
            return "Worker";
        case SDKProfile::WPF:
            return "WPF";
        default:
            throw std::runtime_error(
                "Unhandled enum value in `sdk_profile_to_string`.");

    }
}


SDKProfile sdk_profile_from_string(std::string_view sStr) {
    if (sStr == "Core") {
        return SDKProfile::Core;
    }
    else if (sStr == "None") {
        return SDKProfile::None;
    }
    else if (sStr == "Web") {
        return SDKProfile::Web;
    }
    else if (sStr == "WinForms") {
        return SDKProfile::WinForms;
    }
    else if (sStr == "Worker") {
        return SDKProfile::Worker;
    }
    else if (sStr == "WPF") {
        return SDKProfile::WPF;
    }
    else {
        throw std::runtime_error( "Invalid enum string value in `sdk_profile_from_string`.");
    }
}


} // namespace astfri::csharp
