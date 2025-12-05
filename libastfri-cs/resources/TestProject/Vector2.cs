//
// public class Vector2(float x, float y)
// {
//     // [Obsolete]
//     // public static readonly Vector2 zero = new(0,0);
//     public float X { get; set; } = x;
//     public float Y { get; set; } = y;
//
//     public float this[int i]
//     {
//         get => i switch
//         {
//             0 => X,
//             1 => Y,
//             _ => throw new ArgumentException($"Invalid index : {i}")
//         };
//         set
//         {
//             switch (i)
//             {
//                 case 0:
//                 {
//                     X = value;
//                     break;
//                 }
//                 case 1:
//                     Y = value;
//                     break;
//                 default: throw new ArgumentException($"Invalid index : {i}");
//             }
//         }
//     }
//
//     public static Vector2 operator +(Vector2 first, Vector2 second) =>
//         new(first.X + second.X, first.Y + second.Y);
//
//     public static explicit operator int(Vector2 v) => (int)v.X;
// }