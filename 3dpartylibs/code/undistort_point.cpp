/*

  CODE FOUND AT http://tech.groups.yahoo.com/group/OpenCV/message/26019
  by Philip Gruebele

  With this code snippet as a guide I finally fixed my Undistortion mapping function :P
  Original OpenCV documentation on this is bad :P

*/



bool Utility::UndistortPixel(CvPoint2D32f *src, CvPoint2D32f *dst, CameraParams &intrinsics)
{
      double k1, k2, p1, p2;        // distortion values
      double k_radial;
      double x, y;
      double delta_x, delta_y;
      double r_2;                         // radial distance squared
      double distx, disty;          // distored xy
      CvPoint2D64d desPnt;
      CvPoint2D64d dstd;
      CvPoint2D32f old_src = *src;  // copy of the original distorted point

      // distortion coefficients
      k1 = intrinsics.distortion[0];
      k2 = intrinsics.distortion[1];
      p1 = intrinsics.distortion[2];
      p2 = intrinsics.distortion[3];

    // Shift points to principal point and use focal length
      dstd.x = (src->x - intrinsics.principalPoint[0]) / intrinsics.focalLength[0];
      dstd.y = (src->y - intrinsics.principalPoint[1]) / intrinsics.focalLength[1];
      desPnt.x = dstd.x;
      desPnt.y = dstd.y;

    // Compensate lens distortion
      x = dstd.x;
      y = dstd.y;

    for(int iter = 0; iter < 5; iter++)
    {
        r_2 = x * x + y * y;
        k_radial = 1 + k1 * r_2 + k2 * r_2 * r_2;
        delta_x = 2 * p1 * x * y + p2 * (r_2 + 2 * x * x);
        delta_y = 2 * p2 * x * y + p1 * (r_2 + 2 * y * y);

        x = (desPnt.x - delta_x) / k_radial;
        y = (desPnt.y - delta_y) / k_radial;
    }

    dstd.x = x;
    dstd.y = y;

    dstd.x *= intrinsics.focalLength[0];
    dstd.y *= intrinsics.focalLength[1];

    dstd.x += intrinsics.principalPoint[0];
    dstd.y += intrinsics.principalPoint[1];

    dst->x = (float)dstd.x;
    dst->y = (float)dstd.y;

 

      // do a sanity check to make sure this ideal point translates properly to the distorted point

      // this does the reverse of the above.  It maps ideal undistorted to distorted image coordinates

      x = dstd.x - intrinsics.principalPoint[0];
      y = dstd.y - intrinsics.principalPoint[1];

      x /= intrinsics.focalLength[0];
      y /= intrinsics.focalLength[1];

      r_2 = x*x + y*y;
      distx = x + x*(k1*r_2 + k2*r_2*r_2) + (2*p1*x*y + p2*(r_2 + 2*x*x));
      disty = y + y*(k1*r_2 + k2*r_2*r_2) + (2*p2*x*y + p1*(r_2 + 2*y*y));

      distx *= intrinsics.focalLength[0];
      disty *= intrinsics.focalLength[1];
      distx += intrinsics.principalPoint[0];
      disty += intrinsics.principalPoint[1];

 

      // this should never be more than .2 pixels...

      double diffx = old_src.x - distx;
      double diffy = old_src.y - disty;

      if (fabs(diffx) > .1 || fabs(diffy) > .1)
      {
            char str[100];
            sprintf(str, "diff=%.3g,%.3g", diffx, diffy);
            g_tteyes->Log(str);
            return true;
      }

      return false;

}
