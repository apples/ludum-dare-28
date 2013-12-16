#include "ecposition.hpp"

void  adjustVelocity(ECPosition& a, const ECPosition& b)
{
    double a_top=a.y+a.height/2;
    double a_bottom=a_top-a.height;
    double b_top=b.y+b.height/2;
    double b_bottom=b_top-b.height;

    double a_right=a.x+a.width/2;
    double a_left=a_right-a.width;
    double b_right=b.x+b.width/2;
    double b_left=b_right-b.width;



        if( ((a_bottom>b_bottom&&a_bottom<b_top)|| (a_top>b_bottom&&a_top<b_top)|| (b_bottom>a_bottom&&b_bottom<a_top) || (a_bottom==b_bottom&&a_top==b_top)) && a.dx)
        {

        double a_left_new=a_left+a.dx;
        double a_right_new=a_right+a.dx;
        if( (a_left_new>b_left&&a_left_new<b_right)
            ||(a_right_new>b_left&&a_right_new<b_right)
           || (a_right<b_left&&a_left_new>b_right)
           || (a_left>b_right&&a_right_new<b_left)
           )
            {
                if(a.dy<0) a.dx=b_right-a_left;
                else a.dx=b_left-a_right;

            }
            if(a_right==b_left&&a.dx>0)a.dx=0;
            if(a_left==b_right&&a.dx<0)a.dx=0;
        }

   if( ((a_left>b_left&&a_left<b_right)|| (a_right>b_left&&a_right<b_right)|| (b_left>a_left&&b_left<a_right) || (a_left==b_left&&a_right==b_right)) && a.dy)
        {
        double a_bottom_new=a_bottom+a.dy;
        double a_top_new=a_top+a.dy;
        if( (a_bottom_new>b_bottom&&a_bottom_new<b_top)
            ||(a_top_new>b_bottom&&a_top_new<b_top)
           || (a_top<b_bottom&&a_bottom_new>b_top)
           || (a_bottom>b_top&&a_top_new<b_bottom)
           )
            {
                if(a.dy<0) a.dy=b_top-a_bottom;
                else a.dy=b_bottom-a_top;

            }
            if(a_top==b_bottom&&a.dy>0)a.dy=0;
            if(a_bottom==b_top&&a.dy<0)a.dy=0;

    }


}

