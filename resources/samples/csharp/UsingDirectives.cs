global using M = System.Math;
global using S = System;
global using A = AnotherNamespace.SubNamespace;
global using TestClassALias = AnotherNamespace.SubNamespace.TestClass;

global using GlobalAlias = AnotherNamespace;
using Alias = AnotherNamespace;

using MyNamespace = System.Collections;

using System;
using System.Numerics;
using System.Runtime.CompilerServices;
using MyAlias = System.Collections.Generic.Dictionary<string, int>;


class OtherOuterClass
{
    public class OtherInnerClass
    {
        
    }    
}

class OuterClass
{
    public class OtherInnerClass
    {
        
    }
    public class InnerClass : OtherOuterClass
    {
        public class OtherInnerClass
        {

        }
        private OtherInnerClass otherInnerClass; 
        private OuterClass.OtherInnerClass otherOuterInnerClass;
    }
}

        // This referes to OtherInnerClass in OtherOuterClass - it's inhereted from OuterClass
namespace AnotherNamespace
{
    using OuterAlias = GlobalAlias::SubNamespace;
    using InnerClassAlias = OuterClass.InnerClass;

    public class BaseClass
    {
        // OtherInnerClass otherInnerClass;
    }

    namespace AnotherSubNamespace
    {
        namespace SubNamespace
        {
            
        }
    }

    namespace SubNamespace
    {
        using static OuterClass;
        using AnotherNamespaceAlias = AnotherNamespace.SubNamespace;
        using BaseClassAlias = OuterAlias.BaseClass;
        using TestClassAlias = TestClass;
        public class BaseClass
        {
            
        }
        public class TestClass : BaseClass
        {
            
        }
    };
};