#ifndef CSHARP_SDK_PROFILE_HPP
#define CSHARP_SDK_PROFILE_HPP

/**
 * @brief Enum representing default SDK profiles in C#. Used for determining
 * which standard implicit usings to use
 * @note Implicit usings implemented based on C# documentation. Source:
 * https://learn.microsoft.com/en-us/dotnet/core/project-sdk/overview#implicit-using-directives
 */
enum class SDKProfile {
    /**
     * @brief No SDK profile, no standard implicit usings will be used.
     */
    None,
    /**
     * @brief Microsoft.NET.Sdk profile. Includes this usings:
     * - \code System\endcode
     * - \code System.Collections.Generic\endcode
     * - \code System.IO\endcode
     * - \code System.Linq\endcode
     * - \code System.Net.Http\endcode
     * - \code System.Threading\endcode
     * - \code System.Threading.Tasks\endcode
     */
    Core,
    /**
     * @brief Microsoft.NET.Sdk.Web profile. Includes all usings from
     * \c SDKProfile::NET profile and following additional usings:
     * - \code System.Net.Http.Json\endcode
     * - \code System.Net.Http.Json\endcode
     * - \code Microsoft.AspNetCore.Builder \endcode
     * - \code Microsoft.AspNetCore.Hosting \endcode
     * - \code Microsoft.AspNetCore.Http \endcode
     * - \code Microsoft.AspNetCore.Routing \endcode
     * - \code Microsoft.Extensions.Configuration \endcode
     * - \code Microsoft.Extensions.DependencyInjection \endcode
     * - \code Microsoft.Extensions.Hosting \endcode
     * - \code Microsoft.Extensions.Logging \endcode
     */
    Web,
    /**
     * @brief Microsoft.NET.Sdk.Worker profile. Includes all usings from
     * \c SDKProfile::NET profile and following additional usings:
     * - \code Microsoft.Extensions.Configuration\endcode
     * - \code Microsoft.Extensions.DependencyInjection\endcode
     * - \code Microsoft.Extensions.Hosting\endcode
     * - \code Microsoft.Extensions.Logging\endcode
     */
    Worker,
    /**
     * @brief Microsoft.NET.Sdk.WindowsDesktop profile. Includes all usings from
     * \c SDKProfile::NET profile and following additional usings:
     * - \code System.Drawing\endcode
     * - \code System.Windows.Forms\endcode
     */
    WinForms,
    /**
     * @brief Microsoft.NET.Sdk.WindowsDesktop profile. Includes all usings from
     * \c SDKProfile::NET profile, but excludes:
     * - \code System.IO\endcode
     * - \code System.Net.Http\endcode
     */
    WPF
};

#endif // CSHARP_SDK_PROFILE_HPP