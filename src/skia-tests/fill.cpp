#include <config.h>

#include <math.h>
#include <unistd.h>

#include <SkCanvas.h>
#include <SkPaint.h>
#include <effects/SkGradientShader.h>
#include <SkUnitMappers.h>
#include <SkDraw.h>
#include <effects/SkGradientShader.h>
#include <SkGraphics.h>
#include <SkImageDecoder.h>
#include <SkDashPathEffect.h>

#include "caskbench.h"
#include "skia-shapes.h"
#include "kinetics.h"

SkAutoTUnref<SkPathEffect> pE;
static kinetics_t *skia_particles;
static int element_spacing;
static int num_x_elements;
static int num_y_elements;

int
sk_setup_fill(caskbench_context_t *ctx)
{
    if (ctx->size < 0)
        return 0;

    element_spacing = sqrt( ((double)ctx->canvas_width * ctx->canvas_height) / ctx->size);
    num_x_elements = ctx->canvas_width / element_spacing;
    num_y_elements = ctx->canvas_height / element_spacing;

    return 1;
}

void
sk_teardown_fill(void)
{
}


void apply_stroke_settings(caskbench_context_t *ctx)
{
	//Stroke styles
	SkScalar dashed[] = { SkIntToScalar(20), SkIntToScalar(5)  };
	SkScalar dotted[] = { SkIntToScalar(1), SkIntToScalar(1)  };
	if(ctx->shape_args.stroke_width)
	{

		    ctx->skia_paint->setStyle(SkPaint::kStroke_Style);

			ctx->skia_paint->setStrokeWidth(ctx->shape_args.stroke_width);


			ctx->skia_paint->setStrokeJoin((SkPaint::Join)ctx->shape_args.join_style);
			ctx->skia_paint->setStrokeCap((SkPaint::Cap)ctx->shape_args.cap_style);
			if(ctx->shape_args.dash_style == 0)
			{
					ctx->skia_paint->setPathEffect(NULL);
#if USE_LEGACY_SKIA_SRA
					ctx->skia_paint->setPathEffect(new SkDashPathEffect(dotted, 2, 0))->unref();
#else
					ctx->skia_paint->setPathEffect(SkDashPathEffect::Create(dotted, 2, 0))->unref();
#endif
			}
			else if(ctx->shape_args.dash_style == 1)
			{
					ctx->skia_paint->setPathEffect(NULL);
#if USE_LEGACY_SKIA_SRA
					ctx->skia_paint->setPathEffect(new SkDashPathEffect(dashed, 2, 0))->unref();
#else
					ctx->skia_paint->setPathEffect(SkDashPathEffect::Create(dashed, 2, 0))->unref();
#endif
			}

	}
}

void drawSkiashapes(caskbench_context_t *ctx,kinetics_t *particles)
{
    int i, j, x, y, r, p, shape;
    unsigned int red,green,blue,alpha;
    r = 0.9 * element_spacing /2;
    int old_x, old_y,old_width, old_height;

    int px,py;
    shape = ctx->shape_args.shape_id;
    if(!shape)
    {
        shape = ((4.0 * rand())/RAND_MAX) + 1;
        if(!ctx->shape_args.multi_shapes)
            shape = 2;
    }

    if(shape)
    {
        if(!(ctx->shape_args.center_x && ctx->shape_args.center_y))
        {
            ctx->shape_args.center_x = x = ctx->canvas_width/2;
            ctx->shape_args.center_y = y = ctx->canvas_height/2;
        }
        if(!ctx->shape_args.multi_shapes)
            if(!(ctx->shape_args.width && ctx->shape_args.height))
            {
                ctx->shape_args.width = 100;
                ctx->shape_args.height = 50;
            }
        ctx->shape_args.radius = r;
    }

    SkUnitMapper* linearMapper;
    SkColor linearColors[2];
    SkPoint linearPoints[2];
    SkScalar radialPoints[4];
    SkShader* shader;
    SkPoint center;

    old_width = ctx->shape_args.width;
    old_height = ctx->shape_args.height;
    if(!ctx->shape_args.animation && !ctx->shape_args.multi_shapes)
    {
        num_x_elements = 1;
        num_y_elements = 1;

        r = 40;
        old_x = ctx->shape_args.center_x;
        old_y = ctx->shape_args.center_y;
    }

    for (j=0; j<num_y_elements; j++) {
        y = particles?particles->y : j * element_spacing;
        for (i=0; i<num_x_elements; i++) {
            x = particles?particles->x : i * element_spacing;

            // Options for fill,gradient and transparency
            if(ctx->shape_args.fill_type == none || ctx->shape_args.fill_type == solid)
            {
                if(ctx->shape_args.red > 0 || ctx->shape_args.blue > 0 || ctx->shape_args.green > 0 || ctx->shape_args.alpha > 0)
                {
                    ctx->skia_paint->setARGB(255*(ctx->shape_args.alpha ? (double)ctx->shape_args.alpha:(double)1),
                                             255*ctx->shape_args.red,
                                             255*ctx->shape_args.green,
                                             255*ctx->shape_args.blue);
                }
                else
                    skiaRandomizeColor(ctx);
            }
            else if((ctx->shape_args.fill_type == linearGradient) || (ctx->shape_args.fill_type == radialGradient))
            {
                red = 255*(double)rand()/RAND_MAX;
                green = 255*(double)rand()/RAND_MAX;
                blue = 255*(double)rand()/RAND_MAX;
                alpha = 255*(double)rand()/RAND_MAX;

            }
            else if(ctx->shape_args.fill_type == herringbonePattern){}
            else if((ctx->shape_args.fill_type == imagePattern) && (ctx->shape_args.image_path))
            {
                SkBitmap    bm;

                SkImageDecoder::DecodeFile(ctx->shape_args.image_path, &bm);
                shader = SkShader::CreateBitmapShader(bm, SkShader::kClamp_TileMode,
                                        SkShader::kClamp_TileMode);

                ctx->skia_paint->setShader (shader);
            }
            switch (shape) {
            case 1:
                // Circle
                ctx->shape_args.center_x = (ctx->shape_args.multi_shapes || ctx->shape_args.animation)?x+r:ctx->shape_args.center_x;
                ctx->shape_args.center_y = (ctx->shape_args.multi_shapes || ctx->shape_args.animation)?y+r:ctx->shape_args.center_y;
                ctx->shape_args.radius = r;

                if(ctx->shape_args.fill_type != none)
                {
                    if(ctx->shape_args.fill_type == radialGradient)
                    {
                        linearColors[1] = SkColorSetARGB (alpha, red, green, blue);
                        linearColors[0] = SkColorSetARGB (200,200,200,200);
                        center.set(ctx->shape_args.center_x, ctx->shape_args.center_y);

                        shader = SkGradientShader::CreateRadial (center,r,
                                            linearColors, NULL,2,
                                            SkShader::kClamp_TileMode, NULL);
                        ctx->skia_paint->setShader (shader);
                    }
                    else if(ctx->shape_args.fill_type == linearGradient)
                    {
                        linearColors[0] = SkColorSetARGB (alpha, red, green, blue);
                        linearColors[1] = SkColorSetARGB (200,200,200,200);
                        linearPoints[0].fX = SkIntToScalar(0);
                        linearPoints[0].fY = SkIntToScalar(ctx->shape_args.center_y-r);
                        linearPoints[1].fX = SkIntToScalar(0);
                        linearPoints[1].fY = SkIntToScalar(ctx->shape_args.center_y+r);

                        shader = SkGradientShader::CreateLinear (
                                            linearPoints, linearColors, NULL, 2,
                                            SkShader::kClamp_TileMode, NULL);


                        ctx->skia_paint->setShader (shader);
                    }
                }
		    	ctx->skia_paint->setStyle(SkPaint::kFill_Style);
                skiaShapes[Circle](ctx,&ctx->shape_args);

				if(ctx->shape_args.stroke_width)
				{
					apply_stroke_settings(ctx);
            		ctx->skia_paint->setShader (NULL);
		    		ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
					//Stroke Color
					skiaRandomizeColor(ctx);
					skiaShapes[Circle](ctx,&ctx->shape_args);
				}

                break;

            case 2:
                // Rectangle
                ctx->shape_args.center_x =  (ctx->shape_args.multi_shapes || ctx->shape_args.animation)?x:ctx->shape_args.center_x;
                ctx->shape_args.center_y = (ctx->shape_args.multi_shapes|| ctx->shape_args.animation)?y:ctx->shape_args.center_y;
                ctx->shape_args.width = (ctx->shape_args.center_x) +((ctx->shape_args.width)?ctx->shape_args.width:2*r);
                ctx->shape_args.height = (ctx->shape_args.center_y) + ((ctx->shape_args.height)?ctx->shape_args.height:2*r);

            	if(ctx->shape_args.fill_type != none)
                {
                    if(ctx->shape_args.fill_type == radialGradient)
                    {
                        linearColors[1] = SkColorSetARGB (alpha, red, green, blue);
                        linearColors[0] = SkColorSetARGB (200,200,200,200);
                        center.set((ctx->shape_args.center_x+ctx->shape_args.width)/2, (ctx->shape_args.center_y+ctx->shape_args.height)/2);

                        shader = SkGradientShader::CreateRadial (center, (ctx->shape_args.height-ctx->shape_args.center_y)/2,
                                                                 linearColors, NULL,2,
                                                                 SkShader::kClamp_TileMode, NULL);

                        ctx->skia_paint->setShader (shader);
                    }
                    else if(ctx->shape_args.fill_type == linearGradient)
                    {
                        linearColors[0] = SkColorSetARGB (alpha, red, green, blue);
                        linearColors[1] = SkColorSetARGB (200,200,200,200);
                        linearPoints[0].fX = SkIntToScalar(0);
                        linearPoints[0].fY = SkIntToScalar(ctx->shape_args.center_y);
                        linearPoints[1].fX = SkIntToScalar(0);
                        linearPoints[1].fY = SkIntToScalar(ctx->shape_args.center_y+ctx->shape_args.height/4);

                        shader = SkGradientShader::CreateLinear (
                            linearPoints, linearColors, NULL, 2,
                            SkShader::kClamp_TileMode, NULL);

                        ctx->skia_paint->setShader (shader);
                    }
                }
		    	ctx->skia_paint->setStyle(SkPaint::kFill_Style);
               	skiaShapes[Rectangle](ctx,&ctx->shape_args);

				if(ctx->shape_args.stroke_width)
				{
            		ctx->skia_paint->setShader (NULL);
					apply_stroke_settings(ctx);
		    		ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
					//Stroke Color
					skiaRandomizeColor(ctx);
               		skiaShapes[Rectangle](ctx,&ctx->shape_args);
				}

                ctx->shape_args.width = old_width;
                ctx->shape_args.height = old_height;
                break;

            case 3:
                // Triangle
                if( (!ctx->shape_args.multi_shapes && !ctx->shape_args.animation))
                {
                    x = ctx->shape_args.center_x;
                    y = ctx->shape_args.center_y;
                }

                ctx->shape_args.center_x = x;
                ctx->shape_args.center_y = y;

                if(ctx->shape_args.fill_type != none)
                {
                    if(ctx->shape_args.fill_type == radialGradient)
                    {
                        linearColors[1] = SkColorSetARGB (alpha, red, green, blue);
                        linearColors[0] = SkColorSetARGB (200,200,200,200);
                        center.set(x,(y+2*r));

                        shader = SkGradientShader::CreateRadial (center, r,
                            linearColors, NULL,2,
                            SkShader::kClamp_TileMode, NULL);

                        ctx->skia_paint->setShader (shader);
                    }
                    else if(ctx->shape_args.fill_type == linearGradient)
                    {
                        linearColors[0] = SkColorSetARGB (alpha, red, green, blue);
                        linearColors[1] = SkColorSetARGB (200,200,200,200);
                        linearPoints[0].fX = SkIntToScalar(0);
                        linearPoints[0].fY = SkIntToScalar(y);
                        linearPoints[1].fX = SkIntToScalar(0);
                        linearPoints[1].fY = SkIntToScalar(y+2*r);

                        shader = SkGradientShader::CreateLinear (
                            linearPoints, linearColors, NULL, 2,
                            SkShader::kClamp_TileMode, NULL);

                        ctx->skia_paint->setShader (shader);
                    }
                }

		    	ctx->skia_paint->setStyle(SkPaint::kFill_Style);
				skiaShapes[Triangle] (ctx,&ctx->shape_args);
				if(ctx->shape_args.stroke_width)
				{
            		ctx->skia_paint->setShader (NULL);
					apply_stroke_settings(ctx);
		    		ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
					//Stroke Color
					skiaRandomizeColor(ctx);
					skiaShapes[Triangle] (ctx,&ctx->shape_args);
				}

                break;

            case 4:
                // Star
                if( (!ctx->shape_args.multi_shapes && !ctx->shape_args.animation))
                {
                    x = ctx->shape_args.center_x;
                    y = ctx->shape_args.center_y;
                }

               ctx->shape_args.center_x = x;
               ctx->shape_args.center_y = y;

               px = x + 2*r * star_points[0][0]/200.0;
               py = y + 2*r * star_points[0][1]/200.0;


                if(ctx->shape_args.fill_type != none)
                {
                    if(ctx->shape_args.fill_type == radialGradient)
                    {
                        linearColors[1] = SkColorSetARGB (alpha, red, green, blue);
                        linearColors[0] = SkColorSetARGB (200,200,200,200);

                        center.set(px,py);

                        shader = SkGradientShader::CreateRadial (center, r,
                                                                 linearColors, NULL,2,
                                                                 SkShader::kClamp_TileMode, NULL);

                        ctx->skia_paint->setShader (shader);
                    }
                    else if(ctx->shape_args.fill_type == linearGradient)
                    {
                        linearColors[0] = SkColorSetARGB (alpha, red, green, blue);
                        linearColors[1] = SkColorSetARGB (200,200,200,200);
                        linearPoints[0].fX = SkIntToScalar(0);
                        linearPoints[0].fY = SkIntToScalar(y);
                        linearPoints[1].fX = SkIntToScalar(0);
                        linearPoints[1].fY = SkIntToScalar(y+2*r);

                        shader = SkGradientShader::CreateLinear (
                                                                 linearPoints, linearColors, NULL, 2,
                                                                 SkShader::kClamp_TileMode, NULL);

                        ctx->skia_paint->setShader (shader);
                    }
                }
		    	ctx->skia_paint->setStyle(SkPaint::kFill_Style);
				skiaShapes[Star] (ctx,&ctx->shape_args);

				if(ctx->shape_args.stroke_width)
				{
            		ctx->skia_paint->setShader (NULL);
					apply_stroke_settings(ctx);
		    		ctx->skia_paint->setStyle(SkPaint::kStroke_Style);
					//Stroke Color
					skiaRandomizeColor(ctx);
					skiaShapes[Star] (ctx,&ctx->shape_args);
				}

                break;

            default:
                break;
            }
            ctx->skia_canvas->flush();
            if((ctx->shape_args.fill_type != none) && (ctx->shape_args.fill_type != solid))
                if(shader != NULL)
                {
                    ctx->skia_paint->setShader (NULL);
                    delete shader;
                }

			if(ctx->shape_args.stroke_width)
            	ctx->skia_paint->setPathEffect(NULL);
        }
    }

    if(!ctx->shape_args.animation && !ctx->shape_args.multi_shapes)
    {
        ctx->shape_args.center_x = old_x;
        ctx->shape_args.center_y = old_y;
        ctx->shape_args.width = old_width;
        ctx->shape_args.height = old_height;
    }
}

int
sk_test_fill(caskbench_context_t *ctx)
{
    for(int i = 0;i < ctx->size; i++)
    {
        //Animation / Kinematics of single or multi shape
        if(ctx->shape_args.animation)
        {
            int num_particles = ctx->shape_args.animation;
            double start_frame, stop_frame, delta;
            skia_particles = (kinetics_t *) malloc (sizeof (kinetics_t) * num_particles);
            int i,j ;
            for (i = 0; i < num_particles; i++)
                kinetics_init (&skia_particles[i]);
            delta = 0;

            for (j=0;j<num_particles;j++){
                start_frame = get_tick ();
                ctx->skia_canvas->drawColor(SK_ColorBLACK);

                for (i = 0; i < num_particles; i++) {
                    kinetics_update(&skia_particles[i], 0.1);
                    drawSkiashapes(ctx,&skia_particles[i]);
                }
                stop_frame = get_tick ();
                delta = stop_frame - start_frame;
            }

        }

        // Drawing of multishape on a grid
        else
        {
            drawSkiashapes(ctx,NULL);
        }
    }
    return 1;
}
