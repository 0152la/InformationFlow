; ModuleID = './kgflags.c'
source_filename = "./kgflags.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.anon = type { i32, [256 x %struct._kgflags_flag], i32, [512 x ptr], i32, [512 x %struct._kgflags_error], ptr, i32, i32, ptr, ptr }
%struct._kgflags_flag = type { ptr, ptr, %union.anon, %union.anon.0, i8, i8, i8, i32 }
%union.anon = type { ptr }
%union.anon.0 = type { ptr }
%struct._kgflags_error = type { ptr, ptr, i32 }
%struct.kgflags_string_array = type { ptr, i32 }
%struct.kgflags_int_array = type { ptr, i32 }
%struct.kgflags_double_array = type { ptr, i32 }

@.str = private unnamed_addr constant [4 x i8] c"no-\00", align 1, !dbg !0
@_kgflags_g = internal global %struct.anon zeroinitializer, align 8, !dbg !7
@.str.1 = private unnamed_addr constant [3 x i8] c"--\00", align 1, !dbg !40
@stderr = external global ptr, align 8
@.str.2 = private unnamed_addr constant [30 x i8] c"Missing value for flag: %s%s\0A\00", align 1, !dbg !45
@.str.3 = private unnamed_addr constant [25 x i8] c"Unrecognized flag: %s%s\0A\00", align 1, !dbg !50
@.str.4 = private unnamed_addr constant [32 x i8] c"Unassigned required flag: %s%s\0A\00", align 1, !dbg !55
@.str.5 = private unnamed_addr constant [57 x i8] c"Invalid value for flag: %s%s (got %s, expected integer)\0A\00", align 1, !dbg !60
@.str.6 = private unnamed_addr constant [56 x i8] c"Invalid value for flag: %s%s (got %s, expected number)\0A\00", align 1, !dbg !65
@.str.7 = private unnamed_addr constant [25 x i8] c"Too many flags declared.\00", align 1, !dbg !70
@.str.8 = private unnamed_addr constant [47 x i8] c"Too many non-flag arguments passed to program.\00", align 1, !dbg !72
@.str.9 = private unnamed_addr constant [35 x i8] c"Multiple assignment of flag: %s%s\0A\00", align 1, !dbg !77
@.str.10 = private unnamed_addr constant [29 x i8] c"Redeclaration of flag: %s%s\0A\00", align 1, !dbg !82
@.str.11 = private unnamed_addr constant [53 x i8] c"Used \22no-\22 prefix when declaring boolean flag: %s%s\0A\00", align 1, !dbg !87
@.str.12 = private unnamed_addr constant [14 x i8] c"Usage of %s:\0A\00", align 1, !dbg !92
@.str.13 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1, !dbg !97
@.str.14 = private unnamed_addr constant [8 x i8] c"Flags:\0A\00", align 1, !dbg !99
@.str.15 = private unnamed_addr constant [17 x i8] c"\09%s%s\09(string%s\0A\00", align 1, !dbg !104
@.str.16 = private unnamed_addr constant [2 x i8] c")\00", align 1, !dbg !109
@.str.17 = private unnamed_addr constant [12 x i8] c", optional)\00", align 1, !dbg !114
@.str.18 = private unnamed_addr constant [15 x i8] c"\09\09Default: %s\0A\00", align 1, !dbg !119
@.str.19 = private unnamed_addr constant [27 x i8] c"\09%s%s, %sno-%s\09(boolean%s\0A\00", align 1, !dbg !124
@.str.20 = private unnamed_addr constant [5 x i8] c"True\00", align 1, !dbg !129
@.str.21 = private unnamed_addr constant [6 x i8] c"False\00", align 1, !dbg !134
@.str.22 = private unnamed_addr constant [18 x i8] c"\09%s%s\09(integer%s\0A\00", align 1, !dbg !139
@.str.23 = private unnamed_addr constant [15 x i8] c"\09\09Default: %d\0A\00", align 1, !dbg !144
@.str.24 = private unnamed_addr constant [16 x i8] c"\09%s%s\09(float%s\0A\00", align 1, !dbg !146
@.str.25 = private unnamed_addr constant [18 x i8] c"\09\09Default: %1.4g\0A\00", align 1, !dbg !151
@.str.26 = private unnamed_addr constant [27 x i8] c"\09%s%s\09(array of strings%s\0A\00", align 1, !dbg !153
@.str.27 = private unnamed_addr constant [28 x i8] c"\09%s%s\09(array of integers%s\0A\00", align 1, !dbg !155
@.str.28 = private unnamed_addr constant [26 x i8] c"\09%s%s\09(array of floats%s\0A\00", align 1, !dbg !160
@.str.29 = private unnamed_addr constant [6 x i8] c"\09\09%s\0A\00", align 1, !dbg !165
@.str.30 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1, !dbg !167
@.str.31 = private unnamed_addr constant [7 x i8] c"string\00", align 1, !dbg !169
@.str.32 = private unnamed_addr constant [6 x i8] c"lorem\00", align 1, !dbg !174
@.str.33 = private unnamed_addr constant [13 x i8] c"String flag.\00", align 1, !dbg !176
@.str.34 = private unnamed_addr constant [5 x i8] c"bool\00", align 1, !dbg !181
@.str.35 = private unnamed_addr constant [14 x i8] c"Boolean flag.\00", align 1, !dbg !183
@.str.36 = private unnamed_addr constant [4 x i8] c"int\00", align 1, !dbg !185
@.str.37 = private unnamed_addr constant [14 x i8] c"Integer flag.\00", align 1, !dbg !187
@.str.38 = private unnamed_addr constant [7 x i8] c"double\00", align 1, !dbg !189
@.str.39 = private unnamed_addr constant [13 x i8] c"Double flag.\00", align 1, !dbg !191
@.str.40 = private unnamed_addr constant [13 x i8] c"string-array\00", align 1, !dbg !193
@.str.41 = private unnamed_addr constant [19 x i8] c"String array flag.\00", align 1, !dbg !195
@.str.42 = private unnamed_addr constant [10 x i8] c"int-array\00", align 1, !dbg !200
@.str.43 = private unnamed_addr constant [16 x i8] c"Int array flag.\00", align 1, !dbg !205
@.str.44 = private unnamed_addr constant [13 x i8] c"double-array\00", align 1, !dbg !207
@.str.45 = private unnamed_addr constant [19 x i8] c"Double array flag.\00", align 1, !dbg !209
@.str.46 = private unnamed_addr constant [27 x i8] c"Usage of fullapi [-FLAGS]:\00", align 1, !dbg !211
@.str.47 = private unnamed_addr constant [2 x i8] c"-\00", align 1, !dbg !213
@.str.48 = private unnamed_addr constant [16 x i8] c"string_val: %s\0A\00", align 1, !dbg !215
@.str.49 = private unnamed_addr constant [14 x i8] c"bool_val: %s\0A\00", align 1, !dbg !217
@.str.50 = private unnamed_addr constant [5 x i8] c"true\00", align 1, !dbg !219
@.str.51 = private unnamed_addr constant [6 x i8] c"false\00", align 1, !dbg !221
@.str.52 = private unnamed_addr constant [13 x i8] c"int_val: %d\0A\00", align 1, !dbg !223
@.str.53 = private unnamed_addr constant [16 x i8] c"double_val: %f\0A\00", align 1, !dbg !225
@.str.54 = private unnamed_addr constant [22 x i8] c"string-arr count: %d\0A\00", align 1, !dbg !227
@.str.55 = private unnamed_addr constant [21 x i8] c"string-arr [%d]\0A: %s\00", align 1, !dbg !232
@.str.56 = private unnamed_addr constant [19 x i8] c"int-arr count: %d\0A\00", align 1, !dbg !237
@.str.57 = private unnamed_addr constant [18 x i8] c"int-arr [%d]: %d\0A\00", align 1, !dbg !239
@.str.58 = private unnamed_addr constant [22 x i8] c"double-arr count: %d\0A\00", align 1, !dbg !241
@.str.59 = private unnamed_addr constant [24 x i8] c"double-arr [%d]: %1.4g\0A\00", align 1, !dbg !243
@.str.60 = private unnamed_addr constant [30 x i8] c"Non-flag arguments [%d] = %s\0A\00", align 1, !dbg !248

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @kgflags_string(ptr noundef %0, ptr noundef %1, ptr noundef %2, i1 noundef zeroext %3, ptr noundef %4) #0 !dbg !340 {
  %6 = alloca ptr, align 8
  %7 = alloca ptr, align 8
  %8 = alloca ptr, align 8
  %9 = alloca i8, align 1
  %10 = alloca ptr, align 8
  %11 = alloca %struct._kgflags_flag, align 8
  store ptr %0, ptr %6, align 8
    #dbg_declare(ptr %6, !344, !DIExpression(), !345)
  store ptr %1, ptr %7, align 8
    #dbg_declare(ptr %7, !346, !DIExpression(), !347)
  store ptr %2, ptr %8, align 8
    #dbg_declare(ptr %8, !348, !DIExpression(), !349)
  %12 = zext i1 %3 to i8
  store i8 %12, ptr %9, align 1
    #dbg_declare(ptr %9, !350, !DIExpression(), !351)
  store ptr %4, ptr %10, align 8
    #dbg_declare(ptr %10, !352, !DIExpression(), !353)
  %13 = load ptr, ptr %10, align 8, !dbg !354
  store ptr null, ptr %13, align 8, !dbg !355
    #dbg_declare(ptr %11, !356, !DIExpression(), !357)
  call void @llvm.memset.p0.i64(ptr align 8 %11, i8 0, i64 40, i1 false), !dbg !358
  %14 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 7, !dbg !359
  store i32 1, ptr %14, align 4, !dbg !360
  %15 = load ptr, ptr %6, align 8, !dbg !361
  %16 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 0, !dbg !362
  store ptr %15, ptr %16, align 8, !dbg !363
  %17 = load ptr, ptr %7, align 8, !dbg !364
  %18 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 2, !dbg !365
  store ptr %17, ptr %18, align 8, !dbg !366
  %19 = load ptr, ptr %8, align 8, !dbg !367
  %20 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 1, !dbg !368
  store ptr %19, ptr %20, align 8, !dbg !369
  %21 = load i8, ptr %9, align 1, !dbg !370
  %22 = trunc i8 %21 to i1, !dbg !370
  %23 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 6, !dbg !371
  %24 = zext i1 %22 to i8, !dbg !372
  store i8 %24, ptr %23, align 2, !dbg !372
  %25 = load ptr, ptr %10, align 8, !dbg !373
  %26 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 3, !dbg !374
  store ptr %25, ptr %26, align 8, !dbg !375
  %27 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 4, !dbg !376
  store i8 0, ptr %27, align 8, !dbg !377
  call void @_kgflags_add_flag(ptr noundef byval(%struct._kgflags_flag) align 8 %11), !dbg !378
  ret void, !dbg !379
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @_kgflags_add_flag(ptr noundef byval(%struct._kgflags_flag) align 8 %0) #0 !dbg !380 {
    #dbg_declare(ptr %0, !383, !DIExpression(), !384)
  %2 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %0, i32 0, i32 0, !dbg !385
  %3 = load ptr, ptr %2, align 8, !dbg !385
  %4 = call ptr @_kgflags_get_flag(ptr noundef %3, ptr noundef null), !dbg !387
  %5 = icmp ne ptr %4, null, !dbg !388
  br i1 %5, label %6, label %9, !dbg !388

6:                                                ; preds = %1
  %7 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %0, i32 0, i32 0, !dbg !389
  %8 = load ptr, ptr %7, align 8, !dbg !389
  call void @_kgflags_add_error(i32 noundef 9, ptr noundef %8, ptr noundef null), !dbg !391
  br label %19, !dbg !392

9:                                                ; preds = %1
  %10 = load i32, ptr @_kgflags_g, align 8, !dbg !393
  %11 = icmp sge i32 %10, 256, !dbg !395
  br i1 %11, label %12, label %13, !dbg !395

12:                                               ; preds = %9
  call void @_kgflags_add_error(i32 noundef 6, ptr noundef null, ptr noundef null), !dbg !396
  br label %19, !dbg !398

13:                                               ; preds = %9
  %14 = load i32, ptr @_kgflags_g, align 8, !dbg !399
  %15 = sext i32 %14 to i64, !dbg !400
  %16 = getelementptr inbounds [256 x %struct._kgflags_flag], ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 1), i64 0, i64 %15, !dbg !400
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %16, ptr align 8 %0, i64 40, i1 false), !dbg !401
  %17 = load i32, ptr @_kgflags_g, align 8, !dbg !402
  %18 = add nsw i32 %17, 1, !dbg !402
  store i32 %18, ptr @_kgflags_g, align 8, !dbg !402
  br label %19, !dbg !403

19:                                               ; preds = %13, %12, %6
  ret void, !dbg !403
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @kgflags_bool(ptr noundef %0, i1 noundef zeroext %1, ptr noundef %2, i1 noundef zeroext %3, ptr noundef %4) #0 !dbg !404 {
  %6 = alloca ptr, align 8
  %7 = alloca i8, align 1
  %8 = alloca ptr, align 8
  %9 = alloca i8, align 1
  %10 = alloca ptr, align 8
  %11 = alloca %struct._kgflags_flag, align 8
  store ptr %0, ptr %6, align 8
    #dbg_declare(ptr %6, !407, !DIExpression(), !408)
  %12 = zext i1 %1 to i8
  store i8 %12, ptr %7, align 1
    #dbg_declare(ptr %7, !409, !DIExpression(), !410)
  store ptr %2, ptr %8, align 8
    #dbg_declare(ptr %8, !411, !DIExpression(), !412)
  %13 = zext i1 %3 to i8
  store i8 %13, ptr %9, align 1
    #dbg_declare(ptr %9, !413, !DIExpression(), !414)
  store ptr %4, ptr %10, align 8
    #dbg_declare(ptr %10, !415, !DIExpression(), !416)
  %14 = load ptr, ptr %10, align 8, !dbg !417
  store i8 0, ptr %14, align 1, !dbg !418
  %15 = load ptr, ptr %6, align 8, !dbg !419
  %16 = call ptr @strstr(ptr noundef %15, ptr noundef @.str) #7, !dbg !421
  %17 = load ptr, ptr %6, align 8, !dbg !422
  %18 = icmp eq ptr %16, %17, !dbg !423
  br i1 %18, label %19, label %21, !dbg !423

19:                                               ; preds = %5
  %20 = load ptr, ptr %6, align 8, !dbg !424
  call void @_kgflags_add_error(i32 noundef 10, ptr noundef %20, ptr noundef null), !dbg !426
  br label %38, !dbg !427

21:                                               ; preds = %5
    #dbg_declare(ptr %11, !428, !DIExpression(), !429)
  call void @llvm.memset.p0.i64(ptr align 8 %11, i8 0, i64 40, i1 false), !dbg !430
  %22 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 7, !dbg !431
  store i32 2, ptr %22, align 4, !dbg !432
  %23 = load ptr, ptr %6, align 8, !dbg !433
  %24 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 0, !dbg !434
  store ptr %23, ptr %24, align 8, !dbg !435
  %25 = load i8, ptr %7, align 1, !dbg !436
  %26 = trunc i8 %25 to i1, !dbg !436
  %27 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 2, !dbg !437
  %28 = zext i1 %26 to i8, !dbg !438
  store i8 %28, ptr %27, align 8, !dbg !438
  %29 = load ptr, ptr %8, align 8, !dbg !439
  %30 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 1, !dbg !440
  store ptr %29, ptr %30, align 8, !dbg !441
  %31 = load i8, ptr %9, align 1, !dbg !442
  %32 = trunc i8 %31 to i1, !dbg !442
  %33 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 6, !dbg !443
  %34 = zext i1 %32 to i8, !dbg !444
  store i8 %34, ptr %33, align 2, !dbg !444
  %35 = load ptr, ptr %10, align 8, !dbg !445
  %36 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 3, !dbg !446
  store ptr %35, ptr %36, align 8, !dbg !447
  %37 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 4, !dbg !448
  store i8 0, ptr %37, align 8, !dbg !449
  call void @_kgflags_add_flag(ptr noundef byval(%struct._kgflags_flag) align 8 %11), !dbg !450
  br label %38, !dbg !451

38:                                               ; preds = %21, %19
  ret void, !dbg !451
}

; Function Attrs: nounwind willreturn memory(read)
declare ptr @strstr(ptr noundef, ptr noundef) #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @_kgflags_add_error(i32 noundef %0, ptr noundef %1, ptr noundef %2) #0 !dbg !452 {
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca %struct._kgflags_error, align 8
  store i32 %0, ptr %4, align 4
    #dbg_declare(ptr %4, !455, !DIExpression(), !456)
  store ptr %1, ptr %5, align 8
    #dbg_declare(ptr %5, !457, !DIExpression(), !458)
  store ptr %2, ptr %6, align 8
    #dbg_declare(ptr %6, !459, !DIExpression(), !460)
    #dbg_declare(ptr %7, !461, !DIExpression(), !462)
  %8 = load i32, ptr %4, align 4, !dbg !463
  %9 = getelementptr inbounds nuw %struct._kgflags_error, ptr %7, i32 0, i32 2, !dbg !464
  store i32 %8, ptr %9, align 8, !dbg !465
  %10 = load ptr, ptr %5, align 8, !dbg !466
  %11 = getelementptr inbounds nuw %struct._kgflags_error, ptr %7, i32 0, i32 0, !dbg !467
  store ptr %10, ptr %11, align 8, !dbg !468
  %12 = load ptr, ptr %6, align 8, !dbg !469
  %13 = getelementptr inbounds nuw %struct._kgflags_error, ptr %7, i32 0, i32 1, !dbg !470
  store ptr %12, ptr %13, align 8, !dbg !471
  %14 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 4), align 8, !dbg !472
  %15 = icmp sge i32 %14, 512, !dbg !474
  br i1 %15, label %16, label %17, !dbg !474

16:                                               ; preds = %3
  br label %23, !dbg !475

17:                                               ; preds = %3
  %18 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 4), align 8, !dbg !477
  %19 = sext i32 %18 to i64, !dbg !478
  %20 = getelementptr inbounds [512 x %struct._kgflags_error], ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 5), i64 0, i64 %19, !dbg !478
  call void @llvm.memcpy.p0.p0.i64(ptr align 8 %20, ptr align 8 %7, i64 24, i1 false), !dbg !479
  %21 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 4), align 8, !dbg !480
  %22 = add nsw i32 %21, 1, !dbg !480
  store i32 %22, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 4), align 8, !dbg !480
  br label %23, !dbg !481

23:                                               ; preds = %17, %16
  ret void, !dbg !481
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @kgflags_int(ptr noundef %0, i32 noundef %1, ptr noundef %2, i1 noundef zeroext %3, ptr noundef %4) #0 !dbg !482 {
  %6 = alloca ptr, align 8
  %7 = alloca i32, align 4
  %8 = alloca ptr, align 8
  %9 = alloca i8, align 1
  %10 = alloca ptr, align 8
  %11 = alloca %struct._kgflags_flag, align 8
  store ptr %0, ptr %6, align 8
    #dbg_declare(ptr %6, !485, !DIExpression(), !486)
  store i32 %1, ptr %7, align 4
    #dbg_declare(ptr %7, !487, !DIExpression(), !488)
  store ptr %2, ptr %8, align 8
    #dbg_declare(ptr %8, !489, !DIExpression(), !490)
  %12 = zext i1 %3 to i8
  store i8 %12, ptr %9, align 1
    #dbg_declare(ptr %9, !491, !DIExpression(), !492)
  store ptr %4, ptr %10, align 8
    #dbg_declare(ptr %10, !493, !DIExpression(), !494)
  %13 = load ptr, ptr %10, align 8, !dbg !495
  store i32 0, ptr %13, align 4, !dbg !496
    #dbg_declare(ptr %11, !497, !DIExpression(), !498)
  call void @llvm.memset.p0.i64(ptr align 8 %11, i8 0, i64 40, i1 false), !dbg !499
  %14 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 7, !dbg !500
  store i32 3, ptr %14, align 4, !dbg !501
  %15 = load ptr, ptr %6, align 8, !dbg !502
  %16 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 0, !dbg !503
  store ptr %15, ptr %16, align 8, !dbg !504
  %17 = load i32, ptr %7, align 4, !dbg !505
  %18 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 2, !dbg !506
  store i32 %17, ptr %18, align 8, !dbg !507
  %19 = load ptr, ptr %8, align 8, !dbg !508
  %20 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 1, !dbg !509
  store ptr %19, ptr %20, align 8, !dbg !510
  %21 = load i8, ptr %9, align 1, !dbg !511
  %22 = trunc i8 %21 to i1, !dbg !511
  %23 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 6, !dbg !512
  %24 = zext i1 %22 to i8, !dbg !513
  store i8 %24, ptr %23, align 2, !dbg !513
  %25 = load ptr, ptr %10, align 8, !dbg !514
  %26 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 3, !dbg !515
  store ptr %25, ptr %26, align 8, !dbg !516
  %27 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 4, !dbg !517
  store i8 0, ptr %27, align 8, !dbg !518
  call void @_kgflags_add_flag(ptr noundef byval(%struct._kgflags_flag) align 8 %11), !dbg !519
  ret void, !dbg !520
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @kgflags_double(ptr noundef %0, double noundef %1, ptr noundef %2, i1 noundef zeroext %3, ptr noundef %4) #0 !dbg !521 {
  %6 = alloca ptr, align 8
  %7 = alloca double, align 8
  %8 = alloca ptr, align 8
  %9 = alloca i8, align 1
  %10 = alloca ptr, align 8
  %11 = alloca %struct._kgflags_flag, align 8
  store ptr %0, ptr %6, align 8
    #dbg_declare(ptr %6, !524, !DIExpression(), !525)
  store double %1, ptr %7, align 8
    #dbg_declare(ptr %7, !526, !DIExpression(), !527)
  store ptr %2, ptr %8, align 8
    #dbg_declare(ptr %8, !528, !DIExpression(), !529)
  %12 = zext i1 %3 to i8
  store i8 %12, ptr %9, align 1
    #dbg_declare(ptr %9, !530, !DIExpression(), !531)
  store ptr %4, ptr %10, align 8
    #dbg_declare(ptr %10, !532, !DIExpression(), !533)
  %13 = load ptr, ptr %10, align 8, !dbg !534
  store double 0.000000e+00, ptr %13, align 8, !dbg !535
    #dbg_declare(ptr %11, !536, !DIExpression(), !537)
  call void @llvm.memset.p0.i64(ptr align 8 %11, i8 0, i64 40, i1 false), !dbg !538
  %14 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 7, !dbg !539
  store i32 4, ptr %14, align 4, !dbg !540
  %15 = load ptr, ptr %6, align 8, !dbg !541
  %16 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 0, !dbg !542
  store ptr %15, ptr %16, align 8, !dbg !543
  %17 = load double, ptr %7, align 8, !dbg !544
  %18 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 2, !dbg !545
  store double %17, ptr %18, align 8, !dbg !546
  %19 = load ptr, ptr %8, align 8, !dbg !547
  %20 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 1, !dbg !548
  store ptr %19, ptr %20, align 8, !dbg !549
  %21 = load i8, ptr %9, align 1, !dbg !550
  %22 = trunc i8 %21 to i1, !dbg !550
  %23 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 6, !dbg !551
  %24 = zext i1 %22 to i8, !dbg !552
  store i8 %24, ptr %23, align 2, !dbg !552
  %25 = load ptr, ptr %10, align 8, !dbg !553
  %26 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 3, !dbg !554
  store ptr %25, ptr %26, align 8, !dbg !555
  %27 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 4, !dbg !556
  store i8 0, ptr %27, align 8, !dbg !557
  call void @_kgflags_add_flag(ptr noundef byval(%struct._kgflags_flag) align 8 %11), !dbg !558
  ret void, !dbg !559
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @kgflags_string_array(ptr noundef %0, ptr noundef %1, i1 noundef zeroext %2, ptr noundef %3) #0 !dbg !560 {
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i8, align 1
  %8 = alloca ptr, align 8
  %9 = alloca %struct._kgflags_flag, align 8
  store ptr %0, ptr %5, align 8
    #dbg_declare(ptr %5, !563, !DIExpression(), !564)
  store ptr %1, ptr %6, align 8
    #dbg_declare(ptr %6, !565, !DIExpression(), !566)
  %10 = zext i1 %2 to i8
  store i8 %10, ptr %7, align 1
    #dbg_declare(ptr %7, !567, !DIExpression(), !568)
  store ptr %3, ptr %8, align 8
    #dbg_declare(ptr %8, !569, !DIExpression(), !570)
  %11 = load ptr, ptr %8, align 8, !dbg !571
  %12 = getelementptr inbounds nuw %struct.kgflags_string_array, ptr %11, i32 0, i32 0, !dbg !572
  store ptr null, ptr %12, align 8, !dbg !573
  %13 = load ptr, ptr %8, align 8, !dbg !574
  %14 = getelementptr inbounds nuw %struct.kgflags_string_array, ptr %13, i32 0, i32 1, !dbg !575
  store i32 0, ptr %14, align 8, !dbg !576
    #dbg_declare(ptr %9, !577, !DIExpression(), !578)
  call void @llvm.memset.p0.i64(ptr align 8 %9, i8 0, i64 40, i1 false), !dbg !579
  %15 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 7, !dbg !580
  store i32 5, ptr %15, align 4, !dbg !581
  %16 = load ptr, ptr %5, align 8, !dbg !582
  %17 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 0, !dbg !583
  store ptr %16, ptr %17, align 8, !dbg !584
  %18 = load ptr, ptr %6, align 8, !dbg !585
  %19 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 1, !dbg !586
  store ptr %18, ptr %19, align 8, !dbg !587
  %20 = load i8, ptr %7, align 1, !dbg !588
  %21 = trunc i8 %20 to i1, !dbg !588
  %22 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 6, !dbg !589
  %23 = zext i1 %21 to i8, !dbg !590
  store i8 %23, ptr %22, align 2, !dbg !590
  %24 = load ptr, ptr %8, align 8, !dbg !591
  %25 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 3, !dbg !592
  store ptr %24, ptr %25, align 8, !dbg !593
  %26 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 4, !dbg !594
  store i8 0, ptr %26, align 8, !dbg !595
  call void @_kgflags_add_flag(ptr noundef byval(%struct._kgflags_flag) align 8 %9), !dbg !596
  ret void, !dbg !597
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @kgflags_int_array(ptr noundef %0, ptr noundef %1, i1 noundef zeroext %2, ptr noundef %3) #0 !dbg !598 {
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i8, align 1
  %8 = alloca ptr, align 8
  %9 = alloca %struct._kgflags_flag, align 8
  store ptr %0, ptr %5, align 8
    #dbg_declare(ptr %5, !601, !DIExpression(), !602)
  store ptr %1, ptr %6, align 8
    #dbg_declare(ptr %6, !603, !DIExpression(), !604)
  %10 = zext i1 %2 to i8
  store i8 %10, ptr %7, align 1
    #dbg_declare(ptr %7, !605, !DIExpression(), !606)
  store ptr %3, ptr %8, align 8
    #dbg_declare(ptr %8, !607, !DIExpression(), !608)
  %11 = load ptr, ptr %8, align 8, !dbg !609
  %12 = getelementptr inbounds nuw %struct.kgflags_int_array, ptr %11, i32 0, i32 0, !dbg !610
  store ptr null, ptr %12, align 8, !dbg !611
  %13 = load ptr, ptr %8, align 8, !dbg !612
  %14 = getelementptr inbounds nuw %struct.kgflags_int_array, ptr %13, i32 0, i32 1, !dbg !613
  store i32 0, ptr %14, align 8, !dbg !614
    #dbg_declare(ptr %9, !615, !DIExpression(), !616)
  call void @llvm.memset.p0.i64(ptr align 8 %9, i8 0, i64 40, i1 false), !dbg !617
  %15 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 7, !dbg !618
  store i32 6, ptr %15, align 4, !dbg !619
  %16 = load ptr, ptr %5, align 8, !dbg !620
  %17 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 0, !dbg !621
  store ptr %16, ptr %17, align 8, !dbg !622
  %18 = load ptr, ptr %6, align 8, !dbg !623
  %19 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 1, !dbg !624
  store ptr %18, ptr %19, align 8, !dbg !625
  %20 = load i8, ptr %7, align 1, !dbg !626
  %21 = trunc i8 %20 to i1, !dbg !626
  %22 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 6, !dbg !627
  %23 = zext i1 %21 to i8, !dbg !628
  store i8 %23, ptr %22, align 2, !dbg !628
  %24 = load ptr, ptr %8, align 8, !dbg !629
  %25 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 3, !dbg !630
  store ptr %24, ptr %25, align 8, !dbg !631
  %26 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 4, !dbg !632
  store i8 0, ptr %26, align 8, !dbg !633
  call void @_kgflags_add_flag(ptr noundef byval(%struct._kgflags_flag) align 8 %9), !dbg !634
  ret void, !dbg !635
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @kgflags_double_array(ptr noundef %0, ptr noundef %1, i1 noundef zeroext %2, ptr noundef %3) #0 !dbg !636 {
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i8, align 1
  %8 = alloca ptr, align 8
  %9 = alloca %struct._kgflags_flag, align 8
  store ptr %0, ptr %5, align 8
    #dbg_declare(ptr %5, !639, !DIExpression(), !640)
  store ptr %1, ptr %6, align 8
    #dbg_declare(ptr %6, !641, !DIExpression(), !642)
  %10 = zext i1 %2 to i8
  store i8 %10, ptr %7, align 1
    #dbg_declare(ptr %7, !643, !DIExpression(), !644)
  store ptr %3, ptr %8, align 8
    #dbg_declare(ptr %8, !645, !DIExpression(), !646)
  %11 = load ptr, ptr %8, align 8, !dbg !647
  %12 = getelementptr inbounds nuw %struct.kgflags_double_array, ptr %11, i32 0, i32 0, !dbg !648
  store ptr null, ptr %12, align 8, !dbg !649
  %13 = load ptr, ptr %8, align 8, !dbg !650
  %14 = getelementptr inbounds nuw %struct.kgflags_double_array, ptr %13, i32 0, i32 1, !dbg !651
  store i32 0, ptr %14, align 8, !dbg !652
    #dbg_declare(ptr %9, !653, !DIExpression(), !654)
  call void @llvm.memset.p0.i64(ptr align 8 %9, i8 0, i64 40, i1 false), !dbg !655
  %15 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 7, !dbg !656
  store i32 7, ptr %15, align 4, !dbg !657
  %16 = load ptr, ptr %5, align 8, !dbg !658
  %17 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 0, !dbg !659
  store ptr %16, ptr %17, align 8, !dbg !660
  %18 = load ptr, ptr %6, align 8, !dbg !661
  %19 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 1, !dbg !662
  store ptr %18, ptr %19, align 8, !dbg !663
  %20 = load i8, ptr %7, align 1, !dbg !664
  %21 = trunc i8 %20 to i1, !dbg !664
  %22 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 6, !dbg !665
  %23 = zext i1 %21 to i8, !dbg !666
  store i8 %23, ptr %22, align 2, !dbg !666
  %24 = load ptr, ptr %8, align 8, !dbg !667
  %25 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 3, !dbg !668
  store ptr %24, ptr %25, align 8, !dbg !669
  %26 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %9, i32 0, i32 4, !dbg !670
  store i8 0, ptr %26, align 8, !dbg !671
  call void @_kgflags_add_flag(ptr noundef byval(%struct._kgflags_flag) align 8 %9), !dbg !672
  ret void, !dbg !673
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @kgflags_set_prefix(ptr noundef %0) #0 !dbg !674 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !677, !DIExpression(), !678)
  %3 = load ptr, ptr %2, align 8, !dbg !679
  store ptr %3, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !680
  ret void, !dbg !681
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local zeroext i1 @kgflags_parse(i32 noundef %0, ptr noundef %1) #0 !dbg !682 {
  %3 = alloca i1, align 1
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca ptr, align 8
  %8 = alloca i8, align 1
  %9 = alloca i8, align 1
  %10 = alloca ptr, align 8
  %11 = alloca i32, align 4
  %12 = alloca ptr, align 8
  store i32 %0, ptr %4, align 4
    #dbg_declare(ptr %4, !685, !DIExpression(), !686)
  store ptr %1, ptr %5, align 8
    #dbg_declare(ptr %5, !687, !DIExpression(), !688)
  %13 = load i32, ptr %4, align 4, !dbg !689
  store i32 %13, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 8), align 4, !dbg !690
  %14 = load ptr, ptr %5, align 8, !dbg !691
  store ptr %14, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 9), align 8, !dbg !692
  store i32 1, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 7), align 8, !dbg !693
  %15 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !694
  %16 = icmp eq ptr %15, null, !dbg !696
  br i1 %16, label %17, label %18, !dbg !696

17:                                               ; preds = %2
  store ptr @.str.1, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !697
  br label %18, !dbg !699

18:                                               ; preds = %17, %2
  %19 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 4), align 8, !dbg !700
  %20 = icmp sgt i32 %19, 0, !dbg !702
  br i1 %20, label %21, label %22, !dbg !702

21:                                               ; preds = %18
  store i1 false, ptr %3, align 1, !dbg !703
  br label %94, !dbg !703

22:                                               ; preds = %18
    #dbg_declare(ptr %6, !705, !DIExpression(), !706)
  store ptr null, ptr %6, align 8, !dbg !706
  br label %23, !dbg !707

23:                                               ; preds = %54, %42, %39, %22
  %24 = call ptr @_kgflags_consume_arg(), !dbg !708
  store ptr %24, ptr %6, align 8, !dbg !709
  %25 = icmp ne ptr %24, null, !dbg !710
  br i1 %25, label %26, label %58, !dbg !707

26:                                               ; preds = %23
    #dbg_declare(ptr %7, !711, !DIExpression(), !714)
  store ptr null, ptr %7, align 8, !dbg !714
    #dbg_declare(ptr %8, !715, !DIExpression(), !716)
  %27 = load ptr, ptr %6, align 8, !dbg !717
  %28 = call zeroext i1 @_kgflags_is_flag(ptr noundef %27), !dbg !718
  %29 = zext i1 %28 to i8, !dbg !716
  store i8 %29, ptr %8, align 1, !dbg !716
    #dbg_declare(ptr %9, !719, !DIExpression(), !720)
  store i8 0, ptr %9, align 1, !dbg !720
  %30 = load i8, ptr %8, align 1, !dbg !721
  %31 = trunc i8 %30 to i1, !dbg !721
  br i1 %31, label %32, label %42, !dbg !721

32:                                               ; preds = %26
    #dbg_declare(ptr %10, !723, !DIExpression(), !725)
  %33 = load ptr, ptr %6, align 8, !dbg !726
  %34 = call ptr @_kgflags_get_flag_name(ptr noundef %33), !dbg !727
  store ptr %34, ptr %10, align 8, !dbg !725
  %35 = load ptr, ptr %10, align 8, !dbg !728
  %36 = call ptr @_kgflags_get_flag(ptr noundef %35, ptr noundef %9), !dbg !729
  store ptr %36, ptr %7, align 8, !dbg !730
  %37 = load ptr, ptr %7, align 8, !dbg !731
  %38 = icmp eq ptr %37, null, !dbg !733
  br i1 %38, label %39, label %41, !dbg !733

39:                                               ; preds = %32
  %40 = load ptr, ptr %10, align 8, !dbg !734
  call void @_kgflags_add_error(i32 noundef 2, ptr noundef %40, ptr noundef null), !dbg !736
  br label %23, !dbg !737, !llvm.loop !738

41:                                               ; preds = %32
  br label %45, !dbg !741

42:                                               ; preds = %26
  %43 = load ptr, ptr %6, align 8, !dbg !742
  %44 = call zeroext i1 @_kgflags_add_non_flag_arg(ptr noundef %43), !dbg !744
  br label %23, !dbg !745, !llvm.loop !738

45:                                               ; preds = %41
  %46 = load ptr, ptr %7, align 8, !dbg !746
  %47 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %46, i32 0, i32 4, !dbg !748
  %48 = load i8, ptr %47, align 8, !dbg !748
  %49 = trunc i8 %48 to i1, !dbg !748
  br i1 %49, label %50, label %54, !dbg !746

50:                                               ; preds = %45
  %51 = load ptr, ptr %7, align 8, !dbg !749
  %52 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %51, i32 0, i32 0, !dbg !751
  %53 = load ptr, ptr %52, align 8, !dbg !751
  call void @_kgflags_add_error(i32 noundef 8, ptr noundef %53, ptr noundef null), !dbg !752
  br label %54, !dbg !753

54:                                               ; preds = %50, %45
  %55 = load ptr, ptr %7, align 8, !dbg !754
  %56 = load i8, ptr %9, align 1, !dbg !755
  %57 = trunc i8 %56 to i1, !dbg !755
  call void @_kgflags_parse_flag(ptr noundef %55, i1 noundef zeroext %57), !dbg !756
  br label %23, !dbg !707, !llvm.loop !738

58:                                               ; preds = %23
  call void @_kgflags_assign_default_values(), !dbg !757
    #dbg_declare(ptr %11, !758, !DIExpression(), !760)
  store i32 0, ptr %11, align 4, !dbg !760
  br label %59, !dbg !761

59:                                               ; preds = %86, %58
  %60 = load i32, ptr %11, align 4, !dbg !762
  %61 = load i32, ptr @_kgflags_g, align 8, !dbg !764
  %62 = icmp slt i32 %60, %61, !dbg !765
  br i1 %62, label %63, label %89, !dbg !766

63:                                               ; preds = %59
    #dbg_declare(ptr %12, !767, !DIExpression(), !769)
  %64 = load i32, ptr %11, align 4, !dbg !770
  %65 = sext i32 %64 to i64, !dbg !771
  %66 = getelementptr inbounds [256 x %struct._kgflags_flag], ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 1), i64 0, i64 %65, !dbg !771
  store ptr %66, ptr %12, align 8, !dbg !769
  %67 = load ptr, ptr %12, align 8, !dbg !772
  %68 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %67, i32 0, i32 6, !dbg !774
  %69 = load i8, ptr %68, align 2, !dbg !774
  %70 = trunc i8 %69 to i1, !dbg !774
  br i1 %70, label %71, label %85, !dbg !775

71:                                               ; preds = %63
  %72 = load ptr, ptr %12, align 8, !dbg !776
  %73 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %72, i32 0, i32 4, !dbg !777
  %74 = load i8, ptr %73, align 8, !dbg !777
  %75 = trunc i8 %74 to i1, !dbg !777
  br i1 %75, label %85, label %76, !dbg !778

76:                                               ; preds = %71
  %77 = load ptr, ptr %12, align 8, !dbg !779
  %78 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %77, i32 0, i32 5, !dbg !780
  %79 = load i8, ptr %78, align 1, !dbg !780
  %80 = trunc i8 %79 to i1, !dbg !780
  br i1 %80, label %85, label %81, !dbg !778

81:                                               ; preds = %76
  %82 = load ptr, ptr %12, align 8, !dbg !781
  %83 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %82, i32 0, i32 0, !dbg !783
  %84 = load ptr, ptr %83, align 8, !dbg !783
  call void @_kgflags_add_error(i32 noundef 3, ptr noundef %84, ptr noundef null), !dbg !784
  br label %85, !dbg !785

85:                                               ; preds = %81, %76, %71, %63
  br label %86, !dbg !786

86:                                               ; preds = %85
  %87 = load i32, ptr %11, align 4, !dbg !787
  %88 = add nsw i32 %87, 1, !dbg !787
  store i32 %88, ptr %11, align 4, !dbg !787
  br label %59, !dbg !788, !llvm.loop !789

89:                                               ; preds = %59
  %90 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 4), align 8, !dbg !791
  %91 = icmp sgt i32 %90, 0, !dbg !793
  br i1 %91, label %92, label %93, !dbg !793

92:                                               ; preds = %89
  store i1 false, ptr %3, align 1, !dbg !794
  br label %94, !dbg !794

93:                                               ; preds = %89
  store i1 true, ptr %3, align 1, !dbg !796
  br label %94, !dbg !796

94:                                               ; preds = %93, %92, %21
  %95 = load i1, ptr %3, align 1, !dbg !797
  ret i1 %95, !dbg !797
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal ptr @_kgflags_consume_arg() #0 !dbg !798 {
  %1 = alloca ptr, align 8
  %2 = alloca ptr, align 8
  %3 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 7), align 8, !dbg !801
  %4 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 8), align 4, !dbg !803
  %5 = icmp sge i32 %3, %4, !dbg !804
  br i1 %5, label %6, label %7, !dbg !804

6:                                                ; preds = %0
  store ptr null, ptr %1, align 8, !dbg !805
  br label %16, !dbg !805

7:                                                ; preds = %0
    #dbg_declare(ptr %2, !807, !DIExpression(), !808)
  %8 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 9), align 8, !dbg !809
  %9 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 7), align 8, !dbg !810
  %10 = sext i32 %9 to i64, !dbg !811
  %11 = getelementptr inbounds ptr, ptr %8, i64 %10, !dbg !811
  %12 = load ptr, ptr %11, align 8, !dbg !811
  store ptr %12, ptr %2, align 8, !dbg !808
  %13 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 7), align 8, !dbg !812
  %14 = add nsw i32 %13, 1, !dbg !812
  store i32 %14, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 7), align 8, !dbg !812
  %15 = load ptr, ptr %2, align 8, !dbg !813
  store ptr %15, ptr %1, align 8, !dbg !814
  br label %16, !dbg !814

16:                                               ; preds = %7, %6
  %17 = load ptr, ptr %1, align 8, !dbg !815
  ret ptr %17, !dbg !815
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal zeroext i1 @_kgflags_is_flag(ptr noundef %0) #0 !dbg !816 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !819, !DIExpression(), !820)
  %3 = load ptr, ptr %2, align 8, !dbg !821
  %4 = call ptr @_kgflags_get_flag_name(ptr noundef %3), !dbg !822
  %5 = icmp ne ptr %4, null, !dbg !823
  ret i1 %5, !dbg !824
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal ptr @_kgflags_get_flag_name(ptr noundef %0) #0 !dbg !825 {
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  %4 = alloca i64, align 8
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !828, !DIExpression(), !829)
    #dbg_declare(ptr %4, !830, !DIExpression(), !832)
  %5 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !833
  %6 = call i64 @strlen(ptr noundef %5) #7, !dbg !834
  store i64 %6, ptr %4, align 8, !dbg !832
  %7 = load ptr, ptr %3, align 8, !dbg !835
  %8 = call i64 @strlen(ptr noundef %7) #7, !dbg !837
  %9 = load i64, ptr %4, align 8, !dbg !838
  %10 = icmp ult i64 %8, %9, !dbg !839
  br i1 %10, label %11, label %12, !dbg !839

11:                                               ; preds = %1
  store ptr null, ptr %2, align 8, !dbg !840
  br label %23, !dbg !840

12:                                               ; preds = %1
  %13 = load ptr, ptr %3, align 8, !dbg !842
  %14 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !844
  %15 = load i64, ptr %4, align 8, !dbg !845
  %16 = call i32 @strncmp(ptr noundef %13, ptr noundef %14, i64 noundef %15) #7, !dbg !846
  %17 = icmp ne i32 %16, 0, !dbg !847
  br i1 %17, label %18, label %19, !dbg !847

18:                                               ; preds = %12
  store ptr null, ptr %2, align 8, !dbg !848
  br label %23, !dbg !848

19:                                               ; preds = %12
  %20 = load ptr, ptr %3, align 8, !dbg !850
  %21 = load i64, ptr %4, align 8, !dbg !851
  %22 = getelementptr inbounds nuw i8, ptr %20, i64 %21, !dbg !852
  store ptr %22, ptr %2, align 8, !dbg !853
  br label %23, !dbg !853

23:                                               ; preds = %19, %18, %11
  %24 = load ptr, ptr %2, align 8, !dbg !854
  ret ptr %24, !dbg !854
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal ptr @_kgflags_get_flag(ptr noundef %0, ptr noundef %1) #0 !dbg !855 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
    #dbg_declare(ptr %4, !858, !DIExpression(), !859)
  store ptr %1, ptr %5, align 8
    #dbg_declare(ptr %5, !860, !DIExpression(), !861)
  %8 = load ptr, ptr %5, align 8, !dbg !862
  %9 = icmp ne ptr %8, null, !dbg !862
  br i1 %9, label %10, label %12, !dbg !862

10:                                               ; preds = %2
  %11 = load ptr, ptr %5, align 8, !dbg !864
  store i8 0, ptr %11, align 1, !dbg !866
  br label %12, !dbg !867

12:                                               ; preds = %10, %2
    #dbg_declare(ptr %6, !868, !DIExpression(), !870)
  store i32 0, ptr %6, align 4, !dbg !870
  br label %13, !dbg !871

13:                                               ; preds = %56, %12
  %14 = load i32, ptr %6, align 4, !dbg !872
  %15 = load i32, ptr @_kgflags_g, align 8, !dbg !874
  %16 = icmp slt i32 %14, %15, !dbg !875
  br i1 %16, label %17, label %59, !dbg !876

17:                                               ; preds = %13
    #dbg_declare(ptr %7, !877, !DIExpression(), !879)
  %18 = load i32, ptr %6, align 4, !dbg !880
  %19 = sext i32 %18 to i64, !dbg !881
  %20 = getelementptr inbounds [256 x %struct._kgflags_flag], ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 1), i64 0, i64 %19, !dbg !881
  store ptr %20, ptr %7, align 8, !dbg !879
  %21 = load ptr, ptr %4, align 8, !dbg !882
  %22 = load ptr, ptr %7, align 8, !dbg !884
  %23 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %22, i32 0, i32 0, !dbg !885
  %24 = load ptr, ptr %23, align 8, !dbg !885
  %25 = call i32 @strcmp(ptr noundef %21, ptr noundef %24) #7, !dbg !886
  %26 = icmp eq i32 %25, 0, !dbg !887
  br i1 %26, label %27, label %29, !dbg !887

27:                                               ; preds = %17
  %28 = load ptr, ptr %7, align 8, !dbg !888
  store ptr %28, ptr %3, align 8, !dbg !890
  br label %60, !dbg !890

29:                                               ; preds = %17
  %30 = load ptr, ptr %7, align 8, !dbg !891
  %31 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %30, i32 0, i32 7, !dbg !893
  %32 = load i32, ptr %31, align 4, !dbg !893
  %33 = icmp eq i32 %32, 2, !dbg !894
  br i1 %33, label %34, label %55, !dbg !895

34:                                               ; preds = %29
  %35 = load ptr, ptr %4, align 8, !dbg !896
  %36 = call ptr @strstr(ptr noundef %35, ptr noundef @.str) #7, !dbg !897
  %37 = load ptr, ptr %4, align 8, !dbg !898
  %38 = icmp eq ptr %36, %37, !dbg !899
  br i1 %38, label %39, label %55, !dbg !895

39:                                               ; preds = %34
  %40 = load ptr, ptr %4, align 8, !dbg !900
  %41 = getelementptr inbounds i8, ptr %40, i64 3, !dbg !903
  %42 = load ptr, ptr %7, align 8, !dbg !904
  %43 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %42, i32 0, i32 0, !dbg !905
  %44 = load ptr, ptr %43, align 8, !dbg !905
  %45 = call i32 @strcmp(ptr noundef %41, ptr noundef %44) #7, !dbg !906
  %46 = icmp eq i32 %45, 0, !dbg !907
  br i1 %46, label %47, label %54, !dbg !907

47:                                               ; preds = %39
  %48 = load ptr, ptr %5, align 8, !dbg !908
  %49 = icmp ne ptr %48, null, !dbg !908
  br i1 %49, label %50, label %52, !dbg !908

50:                                               ; preds = %47
  %51 = load ptr, ptr %5, align 8, !dbg !911
  store i8 1, ptr %51, align 1, !dbg !913
  br label %52, !dbg !914

52:                                               ; preds = %50, %47
  %53 = load ptr, ptr %7, align 8, !dbg !915
  store ptr %53, ptr %3, align 8, !dbg !916
  br label %60, !dbg !916

54:                                               ; preds = %39
  br label %55, !dbg !917

55:                                               ; preds = %54, %34, %29
  br label %56, !dbg !918

56:                                               ; preds = %55
  %57 = load i32, ptr %6, align 4, !dbg !919
  %58 = add nsw i32 %57, 1, !dbg !919
  store i32 %58, ptr %6, align 4, !dbg !919
  br label %13, !dbg !920, !llvm.loop !921

59:                                               ; preds = %13
  store ptr null, ptr %3, align 8, !dbg !923
  br label %60, !dbg !923

60:                                               ; preds = %59, %52, %27
  %61 = load ptr, ptr %3, align 8, !dbg !924
  ret ptr %61, !dbg !924
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal zeroext i1 @_kgflags_add_non_flag_arg(ptr noundef %0) #0 !dbg !925 {
  %2 = alloca i1, align 1
  %3 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !926, !DIExpression(), !927)
  %4 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 2), align 8, !dbg !928
  %5 = icmp sge i32 %4, 512, !dbg !930
  br i1 %5, label %6, label %7, !dbg !930

6:                                                ; preds = %1
  call void @_kgflags_add_error(i32 noundef 7, ptr noundef null, ptr noundef null), !dbg !931
  store i1 false, ptr %2, align 1, !dbg !933
  br label %14, !dbg !933

7:                                                ; preds = %1
  %8 = load ptr, ptr %3, align 8, !dbg !934
  %9 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 2), align 8, !dbg !935
  %10 = sext i32 %9 to i64, !dbg !936
  %11 = getelementptr inbounds [512 x ptr], ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 3), i64 0, i64 %10, !dbg !936
  store ptr %8, ptr %11, align 8, !dbg !937
  %12 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 2), align 8, !dbg !938
  %13 = add nsw i32 %12, 1, !dbg !938
  store i32 %13, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 2), align 8, !dbg !938
  store i1 true, ptr %2, align 1, !dbg !939
  br label %14, !dbg !939

14:                                               ; preds = %7, %6
  %15 = load i1, ptr %2, align 1, !dbg !940
  ret i1 %15, !dbg !940
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @_kgflags_parse_flag(ptr noundef %0, i1 noundef zeroext %1) #0 !dbg !941 {
  %3 = alloca ptr, align 8
  %4 = alloca i8, align 1
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i8, align 1
  %8 = alloca i32, align 4
  %9 = alloca ptr, align 8
  %10 = alloca i8, align 1
  %11 = alloca double, align 8
  %12 = alloca i32, align 4
  %13 = alloca i32, align 4
  %14 = alloca ptr, align 8
  %15 = alloca ptr, align 8
  %16 = alloca i32, align 4
  %17 = alloca i32, align 4
  %18 = alloca i8, align 1
  %19 = alloca ptr, align 8
  %20 = alloca i8, align 1
  %21 = alloca ptr, align 8
  %22 = alloca i32, align 4
  %23 = alloca i32, align 4
  %24 = alloca i8, align 1
  %25 = alloca ptr, align 8
  %26 = alloca i8, align 1
  %27 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !944, !DIExpression(), !945)
  %28 = zext i1 %1 to i8
  store i8 %28, ptr %4, align 1
    #dbg_declare(ptr %4, !946, !DIExpression(), !947)
  %29 = load ptr, ptr %3, align 8, !dbg !948
  %30 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %29, i32 0, i32 7, !dbg !949
  %31 = load i32, ptr %30, align 4, !dbg !949
  switch i32 %31, label %236 [
    i32 1, label %32
    i32 2, label %49
    i32 3, label %59
    i32 4, label %88
    i32 5, label %117
    i32 6, label %146
    i32 7, label %191
  ], !dbg !950

32:                                               ; preds = %2
    #dbg_declare(ptr %5, !951, !DIExpression(), !954)
  %33 = call ptr @_kgflags_consume_arg(), !dbg !955
  store ptr %33, ptr %5, align 8, !dbg !954
  %34 = load ptr, ptr %5, align 8, !dbg !956
  %35 = icmp ne ptr %34, null, !dbg !956
  br i1 %35, label %42, label %36, !dbg !958

36:                                               ; preds = %32
  %37 = load ptr, ptr %3, align 8, !dbg !959
  %38 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %37, i32 0, i32 5, !dbg !961
  store i8 1, ptr %38, align 1, !dbg !962
  %39 = load ptr, ptr %3, align 8, !dbg !963
  %40 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %39, i32 0, i32 0, !dbg !964
  %41 = load ptr, ptr %40, align 8, !dbg !964
  call void @_kgflags_add_error(i32 noundef 1, ptr noundef %41, ptr noundef null), !dbg !965
  br label %237, !dbg !966

42:                                               ; preds = %32
  %43 = load ptr, ptr %5, align 8, !dbg !967
  %44 = load ptr, ptr %3, align 8, !dbg !968
  %45 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %44, i32 0, i32 3, !dbg !969
  %46 = load ptr, ptr %45, align 8, !dbg !970
  store ptr %43, ptr %46, align 8, !dbg !971
  %47 = load ptr, ptr %3, align 8, !dbg !972
  %48 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %47, i32 0, i32 4, !dbg !973
  store i8 1, ptr %48, align 8, !dbg !974
  br label %237, !dbg !975

49:                                               ; preds = %2
  %50 = load i8, ptr %4, align 1, !dbg !976
  %51 = trunc i8 %50 to i1, !dbg !976
  %52 = xor i1 %51, true, !dbg !978
  %53 = load ptr, ptr %3, align 8, !dbg !979
  %54 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %53, i32 0, i32 3, !dbg !980
  %55 = load ptr, ptr %54, align 8, !dbg !981
  %56 = zext i1 %52 to i8, !dbg !982
  store i8 %56, ptr %55, align 1, !dbg !982
  %57 = load ptr, ptr %3, align 8, !dbg !983
  %58 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %57, i32 0, i32 4, !dbg !984
  store i8 1, ptr %58, align 8, !dbg !985
  br label %237, !dbg !986

59:                                               ; preds = %2
    #dbg_declare(ptr %6, !987, !DIExpression(), !989)
  %60 = call ptr @_kgflags_consume_arg(), !dbg !990
  store ptr %60, ptr %6, align 8, !dbg !989
  %61 = load ptr, ptr %6, align 8, !dbg !991
  %62 = icmp ne ptr %61, null, !dbg !991
  br i1 %62, label %69, label %63, !dbg !993

63:                                               ; preds = %59
  %64 = load ptr, ptr %3, align 8, !dbg !994
  %65 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %64, i32 0, i32 5, !dbg !996
  store i8 1, ptr %65, align 1, !dbg !997
  %66 = load ptr, ptr %3, align 8, !dbg !998
  %67 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %66, i32 0, i32 0, !dbg !999
  %68 = load ptr, ptr %67, align 8, !dbg !999
  call void @_kgflags_add_error(i32 noundef 1, ptr noundef %68, ptr noundef null), !dbg !1000
  br label %237, !dbg !1001

69:                                               ; preds = %59
    #dbg_declare(ptr %7, !1002, !DIExpression(), !1003)
  store i8 0, ptr %7, align 1, !dbg !1003
    #dbg_declare(ptr %8, !1004, !DIExpression(), !1005)
  %70 = load ptr, ptr %6, align 8, !dbg !1006
  %71 = call i32 @_kgflags_parse_int(ptr noundef %70, ptr noundef %7), !dbg !1007
  store i32 %71, ptr %8, align 4, !dbg !1005
  %72 = load i8, ptr %7, align 1, !dbg !1008
  %73 = trunc i8 %72 to i1, !dbg !1008
  br i1 %73, label %81, label %74, !dbg !1010

74:                                               ; preds = %69
  %75 = load ptr, ptr %3, align 8, !dbg !1011
  %76 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %75, i32 0, i32 5, !dbg !1013
  store i8 1, ptr %76, align 1, !dbg !1014
  %77 = load ptr, ptr %3, align 8, !dbg !1015
  %78 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %77, i32 0, i32 0, !dbg !1016
  %79 = load ptr, ptr %78, align 8, !dbg !1016
  %80 = load ptr, ptr %6, align 8, !dbg !1017
  call void @_kgflags_add_error(i32 noundef 4, ptr noundef %79, ptr noundef %80), !dbg !1018
  br label %237, !dbg !1019

81:                                               ; preds = %69
  %82 = load i32, ptr %8, align 4, !dbg !1020
  %83 = load ptr, ptr %3, align 8, !dbg !1021
  %84 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %83, i32 0, i32 3, !dbg !1022
  %85 = load ptr, ptr %84, align 8, !dbg !1023
  store i32 %82, ptr %85, align 4, !dbg !1024
  %86 = load ptr, ptr %3, align 8, !dbg !1025
  %87 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %86, i32 0, i32 4, !dbg !1026
  store i8 1, ptr %87, align 8, !dbg !1027
  br label %237, !dbg !1028

88:                                               ; preds = %2
    #dbg_declare(ptr %9, !1029, !DIExpression(), !1031)
  %89 = call ptr @_kgflags_consume_arg(), !dbg !1032
  store ptr %89, ptr %9, align 8, !dbg !1031
  %90 = load ptr, ptr %9, align 8, !dbg !1033
  %91 = icmp ne ptr %90, null, !dbg !1033
  br i1 %91, label %98, label %92, !dbg !1035

92:                                               ; preds = %88
  %93 = load ptr, ptr %3, align 8, !dbg !1036
  %94 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %93, i32 0, i32 5, !dbg !1038
  store i8 1, ptr %94, align 1, !dbg !1039
  %95 = load ptr, ptr %3, align 8, !dbg !1040
  %96 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %95, i32 0, i32 0, !dbg !1041
  %97 = load ptr, ptr %96, align 8, !dbg !1041
  call void @_kgflags_add_error(i32 noundef 1, ptr noundef %97, ptr noundef null), !dbg !1042
  br label %237, !dbg !1043

98:                                               ; preds = %88
    #dbg_declare(ptr %10, !1044, !DIExpression(), !1045)
  store i8 0, ptr %10, align 1, !dbg !1045
    #dbg_declare(ptr %11, !1046, !DIExpression(), !1047)
  %99 = load ptr, ptr %9, align 8, !dbg !1048
  %100 = call double @_kgflags_parse_double(ptr noundef %99, ptr noundef %10), !dbg !1049
  store double %100, ptr %11, align 8, !dbg !1047
  %101 = load i8, ptr %10, align 1, !dbg !1050
  %102 = trunc i8 %101 to i1, !dbg !1050
  br i1 %102, label %110, label %103, !dbg !1052

103:                                              ; preds = %98
  %104 = load ptr, ptr %3, align 8, !dbg !1053
  %105 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %104, i32 0, i32 5, !dbg !1055
  store i8 1, ptr %105, align 1, !dbg !1056
  %106 = load ptr, ptr %3, align 8, !dbg !1057
  %107 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %106, i32 0, i32 0, !dbg !1058
  %108 = load ptr, ptr %107, align 8, !dbg !1058
  %109 = load ptr, ptr %9, align 8, !dbg !1059
  call void @_kgflags_add_error(i32 noundef 5, ptr noundef %108, ptr noundef %109), !dbg !1060
  br label %237, !dbg !1061

110:                                              ; preds = %98
  %111 = load double, ptr %11, align 8, !dbg !1062
  %112 = load ptr, ptr %3, align 8, !dbg !1063
  %113 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %112, i32 0, i32 3, !dbg !1064
  %114 = load ptr, ptr %113, align 8, !dbg !1065
  store double %111, ptr %114, align 8, !dbg !1066
  %115 = load ptr, ptr %3, align 8, !dbg !1067
  %116 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %115, i32 0, i32 4, !dbg !1068
  store i8 1, ptr %116, align 8, !dbg !1069
  br label %237, !dbg !1070

117:                                              ; preds = %2
    #dbg_declare(ptr %12, !1071, !DIExpression(), !1073)
  %118 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 7), align 8, !dbg !1074
  store i32 %118, ptr %12, align 4, !dbg !1073
    #dbg_declare(ptr %13, !1075, !DIExpression(), !1076)
  store i32 0, ptr %13, align 4, !dbg !1076
  br label %119, !dbg !1077

119:                                              ; preds = %117, %127
    #dbg_declare(ptr %14, !1078, !DIExpression(), !1080)
  %120 = call ptr @_kgflags_peek_arg(), !dbg !1081
  store ptr %120, ptr %14, align 8, !dbg !1080
  %121 = load ptr, ptr %14, align 8, !dbg !1082
  %122 = icmp eq ptr %121, null, !dbg !1084
  br i1 %122, label %126, label %123, !dbg !1085

123:                                              ; preds = %119
  %124 = load ptr, ptr %14, align 8, !dbg !1086
  %125 = call zeroext i1 @_kgflags_is_flag(ptr noundef %124), !dbg !1087
  br i1 %125, label %126, label %127, !dbg !1085

126:                                              ; preds = %123, %119
  br label %131, !dbg !1088

127:                                              ; preds = %123
  %128 = call ptr @_kgflags_consume_arg(), !dbg !1090
  %129 = load i32, ptr %13, align 4, !dbg !1091
  %130 = add nsw i32 %129, 1, !dbg !1091
  store i32 %130, ptr %13, align 4, !dbg !1091
  br label %119, !dbg !1077, !llvm.loop !1092

131:                                              ; preds = %126
    #dbg_declare(ptr %15, !1094, !DIExpression(), !1095)
  %132 = load ptr, ptr %3, align 8, !dbg !1096
  %133 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %132, i32 0, i32 3, !dbg !1097
  %134 = load ptr, ptr %133, align 8, !dbg !1098
  store ptr %134, ptr %15, align 8, !dbg !1095
  %135 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 9), align 8, !dbg !1099
  %136 = load i32, ptr %12, align 4, !dbg !1100
  %137 = sext i32 %136 to i64, !dbg !1101
  %138 = getelementptr inbounds ptr, ptr %135, i64 %137, !dbg !1101
  %139 = load ptr, ptr %15, align 8, !dbg !1102
  %140 = getelementptr inbounds nuw %struct.kgflags_string_array, ptr %139, i32 0, i32 0, !dbg !1103
  store ptr %138, ptr %140, align 8, !dbg !1104
  %141 = load i32, ptr %13, align 4, !dbg !1105
  %142 = load ptr, ptr %15, align 8, !dbg !1106
  %143 = getelementptr inbounds nuw %struct.kgflags_string_array, ptr %142, i32 0, i32 1, !dbg !1107
  store i32 %141, ptr %143, align 8, !dbg !1108
  %144 = load ptr, ptr %3, align 8, !dbg !1109
  %145 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %144, i32 0, i32 4, !dbg !1110
  store i8 1, ptr %145, align 8, !dbg !1111
  br label %237, !dbg !1112

146:                                              ; preds = %2
    #dbg_declare(ptr %16, !1113, !DIExpression(), !1115)
  %147 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 7), align 8, !dbg !1116
  store i32 %147, ptr %16, align 4, !dbg !1115
    #dbg_declare(ptr %17, !1117, !DIExpression(), !1118)
  store i32 0, ptr %17, align 4, !dbg !1118
    #dbg_declare(ptr %18, !1119, !DIExpression(), !1120)
  store i8 1, ptr %18, align 1, !dbg !1120
  br label %148, !dbg !1121

148:                                              ; preds = %146, %169
    #dbg_declare(ptr %19, !1122, !DIExpression(), !1124)
  %149 = call ptr @_kgflags_peek_arg(), !dbg !1125
  store ptr %149, ptr %19, align 8, !dbg !1124
  %150 = load ptr, ptr %19, align 8, !dbg !1126
  %151 = icmp eq ptr %150, null, !dbg !1128
  br i1 %151, label %155, label %152, !dbg !1129

152:                                              ; preds = %148
  %153 = load ptr, ptr %19, align 8, !dbg !1130
  %154 = call zeroext i1 @_kgflags_is_flag(ptr noundef %153), !dbg !1131
  br i1 %154, label %155, label %156, !dbg !1129

155:                                              ; preds = %152, %148
  br label %172, !dbg !1132

156:                                              ; preds = %152
  %157 = call ptr @_kgflags_consume_arg(), !dbg !1134
    #dbg_declare(ptr %20, !1135, !DIExpression(), !1136)
  store i8 0, ptr %20, align 1, !dbg !1136
  %158 = load ptr, ptr %19, align 8, !dbg !1137
  %159 = call i32 @_kgflags_parse_int(ptr noundef %158, ptr noundef %20), !dbg !1138
  %160 = load i8, ptr %20, align 1, !dbg !1139
  %161 = trunc i8 %160 to i1, !dbg !1139
  br i1 %161, label %169, label %162, !dbg !1141

162:                                              ; preds = %156
  %163 = load ptr, ptr %3, align 8, !dbg !1142
  %164 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %163, i32 0, i32 5, !dbg !1144
  store i8 1, ptr %164, align 1, !dbg !1145
  %165 = load ptr, ptr %3, align 8, !dbg !1146
  %166 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %165, i32 0, i32 0, !dbg !1147
  %167 = load ptr, ptr %166, align 8, !dbg !1147
  %168 = load ptr, ptr %19, align 8, !dbg !1148
  call void @_kgflags_add_error(i32 noundef 4, ptr noundef %167, ptr noundef %168), !dbg !1149
  store i8 0, ptr %18, align 1, !dbg !1150
  br label %169, !dbg !1151

169:                                              ; preds = %162, %156
  %170 = load i32, ptr %17, align 4, !dbg !1152
  %171 = add nsw i32 %170, 1, !dbg !1152
  store i32 %171, ptr %17, align 4, !dbg !1152
  br label %148, !dbg !1121, !llvm.loop !1153

172:                                              ; preds = %155
    #dbg_declare(ptr %21, !1155, !DIExpression(), !1156)
  %173 = load ptr, ptr %3, align 8, !dbg !1157
  %174 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %173, i32 0, i32 3, !dbg !1158
  %175 = load ptr, ptr %174, align 8, !dbg !1159
  store ptr %175, ptr %21, align 8, !dbg !1156
  %176 = load i8, ptr %18, align 1, !dbg !1160
  %177 = trunc i8 %176 to i1, !dbg !1160
  br i1 %177, label %178, label %188, !dbg !1160

178:                                              ; preds = %172
  %179 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 9), align 8, !dbg !1162
  %180 = load i32, ptr %16, align 4, !dbg !1164
  %181 = sext i32 %180 to i64, !dbg !1165
  %182 = getelementptr inbounds ptr, ptr %179, i64 %181, !dbg !1165
  %183 = load ptr, ptr %21, align 8, !dbg !1166
  %184 = getelementptr inbounds nuw %struct.kgflags_int_array, ptr %183, i32 0, i32 0, !dbg !1167
  store ptr %182, ptr %184, align 8, !dbg !1168
  %185 = load i32, ptr %17, align 4, !dbg !1169
  %186 = load ptr, ptr %21, align 8, !dbg !1170
  %187 = getelementptr inbounds nuw %struct.kgflags_int_array, ptr %186, i32 0, i32 1, !dbg !1171
  store i32 %185, ptr %187, align 8, !dbg !1172
  br label %188, !dbg !1173

188:                                              ; preds = %178, %172
  %189 = load ptr, ptr %3, align 8, !dbg !1174
  %190 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %189, i32 0, i32 4, !dbg !1175
  store i8 1, ptr %190, align 8, !dbg !1176
  br label %237, !dbg !1177

191:                                              ; preds = %2
    #dbg_declare(ptr %22, !1178, !DIExpression(), !1180)
  %192 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 7), align 8, !dbg !1181
  store i32 %192, ptr %22, align 4, !dbg !1180
    #dbg_declare(ptr %23, !1182, !DIExpression(), !1183)
  store i32 0, ptr %23, align 4, !dbg !1183
    #dbg_declare(ptr %24, !1184, !DIExpression(), !1185)
  store i8 1, ptr %24, align 1, !dbg !1185
  br label %193, !dbg !1186

193:                                              ; preds = %191, %214
    #dbg_declare(ptr %25, !1187, !DIExpression(), !1189)
  %194 = call ptr @_kgflags_peek_arg(), !dbg !1190
  store ptr %194, ptr %25, align 8, !dbg !1189
  %195 = load ptr, ptr %25, align 8, !dbg !1191
  %196 = icmp eq ptr %195, null, !dbg !1193
  br i1 %196, label %200, label %197, !dbg !1194

197:                                              ; preds = %193
  %198 = load ptr, ptr %25, align 8, !dbg !1195
  %199 = call zeroext i1 @_kgflags_is_flag(ptr noundef %198), !dbg !1196
  br i1 %199, label %200, label %201, !dbg !1194

200:                                              ; preds = %197, %193
  br label %217, !dbg !1197

201:                                              ; preds = %197
  %202 = call ptr @_kgflags_consume_arg(), !dbg !1199
    #dbg_declare(ptr %26, !1200, !DIExpression(), !1201)
  store i8 0, ptr %26, align 1, !dbg !1201
  %203 = load ptr, ptr %25, align 8, !dbg !1202
  %204 = call double @_kgflags_parse_double(ptr noundef %203, ptr noundef %26), !dbg !1203
  %205 = load i8, ptr %26, align 1, !dbg !1204
  %206 = trunc i8 %205 to i1, !dbg !1204
  br i1 %206, label %214, label %207, !dbg !1206

207:                                              ; preds = %201
  %208 = load ptr, ptr %3, align 8, !dbg !1207
  %209 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %208, i32 0, i32 5, !dbg !1209
  store i8 1, ptr %209, align 1, !dbg !1210
  %210 = load ptr, ptr %3, align 8, !dbg !1211
  %211 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %210, i32 0, i32 0, !dbg !1212
  %212 = load ptr, ptr %211, align 8, !dbg !1212
  %213 = load ptr, ptr %25, align 8, !dbg !1213
  call void @_kgflags_add_error(i32 noundef 5, ptr noundef %212, ptr noundef %213), !dbg !1214
  store i8 0, ptr %24, align 1, !dbg !1215
  br label %214, !dbg !1216

214:                                              ; preds = %207, %201
  %215 = load i32, ptr %23, align 4, !dbg !1217
  %216 = add nsw i32 %215, 1, !dbg !1217
  store i32 %216, ptr %23, align 4, !dbg !1217
  br label %193, !dbg !1186, !llvm.loop !1218

217:                                              ; preds = %200
    #dbg_declare(ptr %27, !1220, !DIExpression(), !1221)
  %218 = load ptr, ptr %3, align 8, !dbg !1222
  %219 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %218, i32 0, i32 3, !dbg !1223
  %220 = load ptr, ptr %219, align 8, !dbg !1224
  store ptr %220, ptr %27, align 8, !dbg !1221
  %221 = load i8, ptr %24, align 1, !dbg !1225
  %222 = trunc i8 %221 to i1, !dbg !1225
  br i1 %222, label %223, label %233, !dbg !1225

223:                                              ; preds = %217
  %224 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 9), align 8, !dbg !1227
  %225 = load i32, ptr %22, align 4, !dbg !1229
  %226 = sext i32 %225 to i64, !dbg !1230
  %227 = getelementptr inbounds ptr, ptr %224, i64 %226, !dbg !1230
  %228 = load ptr, ptr %27, align 8, !dbg !1231
  %229 = getelementptr inbounds nuw %struct.kgflags_double_array, ptr %228, i32 0, i32 0, !dbg !1232
  store ptr %227, ptr %229, align 8, !dbg !1233
  %230 = load i32, ptr %23, align 4, !dbg !1234
  %231 = load ptr, ptr %27, align 8, !dbg !1235
  %232 = getelementptr inbounds nuw %struct.kgflags_double_array, ptr %231, i32 0, i32 1, !dbg !1236
  store i32 %230, ptr %232, align 8, !dbg !1237
  br label %233, !dbg !1238

233:                                              ; preds = %223, %217
  %234 = load ptr, ptr %3, align 8, !dbg !1239
  %235 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %234, i32 0, i32 4, !dbg !1240
  store i8 1, ptr %235, align 8, !dbg !1241
  br label %237, !dbg !1242

236:                                              ; preds = %2
  br label %237, !dbg !1243

237:                                              ; preds = %36, %63, %74, %92, %103, %236, %233, %188, %131, %110, %81, %49, %42
  ret void, !dbg !1244
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @_kgflags_assign_default_values() #0 !dbg !1245 {
  %1 = alloca i32, align 4
  %2 = alloca ptr, align 8
    #dbg_declare(ptr %1, !1248, !DIExpression(), !1250)
  store i32 0, ptr %1, align 4, !dbg !1250
  br label %3, !dbg !1251

3:                                                ; preds = %65, %0
  %4 = load i32, ptr %1, align 4, !dbg !1252
  %5 = load i32, ptr @_kgflags_g, align 8, !dbg !1254
  %6 = icmp slt i32 %4, %5, !dbg !1255
  br i1 %6, label %7, label %68, !dbg !1256

7:                                                ; preds = %3
    #dbg_declare(ptr %2, !1257, !DIExpression(), !1259)
  %8 = load i32, ptr %1, align 4, !dbg !1260
  %9 = sext i32 %8 to i64, !dbg !1261
  %10 = getelementptr inbounds [256 x %struct._kgflags_flag], ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 1), i64 0, i64 %9, !dbg !1261
  store ptr %10, ptr %2, align 8, !dbg !1259
  %11 = load ptr, ptr %2, align 8, !dbg !1262
  %12 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %11, i32 0, i32 4, !dbg !1264
  %13 = load i8, ptr %12, align 8, !dbg !1264
  %14 = trunc i8 %13 to i1, !dbg !1264
  br i1 %14, label %20, label %15, !dbg !1265

15:                                               ; preds = %7
  %16 = load ptr, ptr %2, align 8, !dbg !1266
  %17 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %16, i32 0, i32 6, !dbg !1267
  %18 = load i8, ptr %17, align 2, !dbg !1267
  %19 = trunc i8 %18 to i1, !dbg !1267
  br i1 %19, label %20, label %21, !dbg !1265

20:                                               ; preds = %15, %7
  br label %65, !dbg !1268

21:                                               ; preds = %15
  %22 = load ptr, ptr %2, align 8, !dbg !1270
  %23 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %22, i32 0, i32 7, !dbg !1271
  %24 = load i32, ptr %23, align 4, !dbg !1271
  switch i32 %24, label %63 [
    i32 1, label %25
    i32 2, label %34
    i32 3, label %45
    i32 4, label %54
  ], !dbg !1272

25:                                               ; preds = %21
  %26 = load ptr, ptr %2, align 8, !dbg !1273
  %27 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %26, i32 0, i32 2, !dbg !1276
  %28 = load ptr, ptr %27, align 8, !dbg !1277
  %29 = load ptr, ptr %2, align 8, !dbg !1278
  %30 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %29, i32 0, i32 3, !dbg !1279
  %31 = load ptr, ptr %30, align 8, !dbg !1280
  store ptr %28, ptr %31, align 8, !dbg !1281
  %32 = load ptr, ptr %2, align 8, !dbg !1282
  %33 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %32, i32 0, i32 4, !dbg !1283
  store i8 1, ptr %33, align 8, !dbg !1284
  br label %64, !dbg !1285

34:                                               ; preds = %21
  %35 = load ptr, ptr %2, align 8, !dbg !1286
  %36 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %35, i32 0, i32 2, !dbg !1288
  %37 = load i8, ptr %36, align 8, !dbg !1289
  %38 = trunc i8 %37 to i1, !dbg !1289
  %39 = load ptr, ptr %2, align 8, !dbg !1290
  %40 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %39, i32 0, i32 3, !dbg !1291
  %41 = load ptr, ptr %40, align 8, !dbg !1292
  %42 = zext i1 %38 to i8, !dbg !1293
  store i8 %42, ptr %41, align 1, !dbg !1293
  %43 = load ptr, ptr %2, align 8, !dbg !1294
  %44 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %43, i32 0, i32 4, !dbg !1295
  store i8 1, ptr %44, align 8, !dbg !1296
  br label %64, !dbg !1297

45:                                               ; preds = %21
  %46 = load ptr, ptr %2, align 8, !dbg !1298
  %47 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %46, i32 0, i32 2, !dbg !1300
  %48 = load i32, ptr %47, align 8, !dbg !1301
  %49 = load ptr, ptr %2, align 8, !dbg !1302
  %50 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %49, i32 0, i32 3, !dbg !1303
  %51 = load ptr, ptr %50, align 8, !dbg !1304
  store i32 %48, ptr %51, align 4, !dbg !1305
  %52 = load ptr, ptr %2, align 8, !dbg !1306
  %53 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %52, i32 0, i32 4, !dbg !1307
  store i8 1, ptr %53, align 8, !dbg !1308
  br label %64, !dbg !1309

54:                                               ; preds = %21
  %55 = load ptr, ptr %2, align 8, !dbg !1310
  %56 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %55, i32 0, i32 2, !dbg !1312
  %57 = load double, ptr %56, align 8, !dbg !1313
  %58 = load ptr, ptr %2, align 8, !dbg !1314
  %59 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %58, i32 0, i32 3, !dbg !1315
  %60 = load ptr, ptr %59, align 8, !dbg !1316
  store double %57, ptr %60, align 8, !dbg !1317
  %61 = load ptr, ptr %2, align 8, !dbg !1318
  %62 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %61, i32 0, i32 4, !dbg !1319
  store i8 1, ptr %62, align 8, !dbg !1320
  br label %64, !dbg !1321

63:                                               ; preds = %21
  br label %64, !dbg !1322

64:                                               ; preds = %63, %54, %45, %34, %25
  br label %65, !dbg !1323

65:                                               ; preds = %64, %20
  %66 = load i32, ptr %1, align 4, !dbg !1324
  %67 = add nsw i32 %66, 1, !dbg !1324
  store i32 %67, ptr %1, align 4, !dbg !1324
  br label %3, !dbg !1325, !llvm.loop !1326

68:                                               ; preds = %3
  ret void, !dbg !1328
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @kgflags_print_errors() #0 !dbg !1329 {
  %1 = alloca i32, align 4
  %2 = alloca ptr, align 8
    #dbg_declare(ptr %1, !1330, !DIExpression(), !1332)
  store i32 0, ptr %1, align 4, !dbg !1332
  br label %3, !dbg !1333

3:                                                ; preds = %84, %0
  %4 = load i32, ptr %1, align 4, !dbg !1334
  %5 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 4), align 8, !dbg !1336
  %6 = icmp slt i32 %4, %5, !dbg !1337
  br i1 %6, label %7, label %87, !dbg !1338

7:                                                ; preds = %3
    #dbg_declare(ptr %2, !1339, !DIExpression(), !1342)
  %8 = load i32, ptr %1, align 4, !dbg !1343
  %9 = sext i32 %8 to i64, !dbg !1344
  %10 = getelementptr inbounds [512 x %struct._kgflags_error], ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 5), i64 0, i64 %9, !dbg !1344
  store ptr %10, ptr %2, align 8, !dbg !1342
  %11 = load ptr, ptr %2, align 8, !dbg !1345
  %12 = getelementptr inbounds nuw %struct._kgflags_error, ptr %11, i32 0, i32 2, !dbg !1346
  %13 = load i32, ptr %12, align 8, !dbg !1346
  switch i32 %13, label %82 [
    i32 1, label %14
    i32 2, label %21
    i32 3, label %28
    i32 4, label %35
    i32 5, label %45
    i32 6, label %55
    i32 7, label %58
    i32 8, label %61
    i32 9, label %68
    i32 10, label %75
  ], !dbg !1347

14:                                               ; preds = %7
  %15 = load ptr, ptr @stderr, align 8, !dbg !1348
  %16 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1351
  %17 = load ptr, ptr %2, align 8, !dbg !1352
  %18 = getelementptr inbounds nuw %struct._kgflags_error, ptr %17, i32 0, i32 0, !dbg !1353
  %19 = load ptr, ptr %18, align 8, !dbg !1353
  %20 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %15, ptr noundef @.str.2, ptr noundef %16, ptr noundef %19) #8, !dbg !1354
  br label %83, !dbg !1355

21:                                               ; preds = %7
  %22 = load ptr, ptr @stderr, align 8, !dbg !1356
  %23 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1358
  %24 = load ptr, ptr %2, align 8, !dbg !1359
  %25 = getelementptr inbounds nuw %struct._kgflags_error, ptr %24, i32 0, i32 0, !dbg !1360
  %26 = load ptr, ptr %25, align 8, !dbg !1360
  %27 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %22, ptr noundef @.str.3, ptr noundef %23, ptr noundef %26) #8, !dbg !1361
  br label %83, !dbg !1362

28:                                               ; preds = %7
  %29 = load ptr, ptr @stderr, align 8, !dbg !1363
  %30 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1365
  %31 = load ptr, ptr %2, align 8, !dbg !1366
  %32 = getelementptr inbounds nuw %struct._kgflags_error, ptr %31, i32 0, i32 0, !dbg !1367
  %33 = load ptr, ptr %32, align 8, !dbg !1367
  %34 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %29, ptr noundef @.str.4, ptr noundef %30, ptr noundef %33) #8, !dbg !1368
  br label %83, !dbg !1369

35:                                               ; preds = %7
  %36 = load ptr, ptr @stderr, align 8, !dbg !1370
  %37 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1372
  %38 = load ptr, ptr %2, align 8, !dbg !1373
  %39 = getelementptr inbounds nuw %struct._kgflags_error, ptr %38, i32 0, i32 0, !dbg !1374
  %40 = load ptr, ptr %39, align 8, !dbg !1374
  %41 = load ptr, ptr %2, align 8, !dbg !1375
  %42 = getelementptr inbounds nuw %struct._kgflags_error, ptr %41, i32 0, i32 1, !dbg !1376
  %43 = load ptr, ptr %42, align 8, !dbg !1376
  %44 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %36, ptr noundef @.str.5, ptr noundef %37, ptr noundef %40, ptr noundef %43) #8, !dbg !1377
  br label %83, !dbg !1378

45:                                               ; preds = %7
  %46 = load ptr, ptr @stderr, align 8, !dbg !1379
  %47 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1381
  %48 = load ptr, ptr %2, align 8, !dbg !1382
  %49 = getelementptr inbounds nuw %struct._kgflags_error, ptr %48, i32 0, i32 0, !dbg !1383
  %50 = load ptr, ptr %49, align 8, !dbg !1383
  %51 = load ptr, ptr %2, align 8, !dbg !1384
  %52 = getelementptr inbounds nuw %struct._kgflags_error, ptr %51, i32 0, i32 1, !dbg !1385
  %53 = load ptr, ptr %52, align 8, !dbg !1385
  %54 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %46, ptr noundef @.str.6, ptr noundef %47, ptr noundef %50, ptr noundef %53) #8, !dbg !1386
  br label %83, !dbg !1387

55:                                               ; preds = %7
  %56 = load ptr, ptr @stderr, align 8, !dbg !1388
  %57 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %56, ptr noundef @.str.7) #8, !dbg !1390
  br label %83, !dbg !1391

58:                                               ; preds = %7
  %59 = load ptr, ptr @stderr, align 8, !dbg !1392
  %60 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %59, ptr noundef @.str.8) #8, !dbg !1394
  br label %83, !dbg !1395

61:                                               ; preds = %7
  %62 = load ptr, ptr @stderr, align 8, !dbg !1396
  %63 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1398
  %64 = load ptr, ptr %2, align 8, !dbg !1399
  %65 = getelementptr inbounds nuw %struct._kgflags_error, ptr %64, i32 0, i32 0, !dbg !1400
  %66 = load ptr, ptr %65, align 8, !dbg !1400
  %67 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %62, ptr noundef @.str.9, ptr noundef %63, ptr noundef %66) #8, !dbg !1401
  br label %83, !dbg !1402

68:                                               ; preds = %7
  %69 = load ptr, ptr @stderr, align 8, !dbg !1403
  %70 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1405
  %71 = load ptr, ptr %2, align 8, !dbg !1406
  %72 = getelementptr inbounds nuw %struct._kgflags_error, ptr %71, i32 0, i32 0, !dbg !1407
  %73 = load ptr, ptr %72, align 8, !dbg !1407
  %74 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %69, ptr noundef @.str.10, ptr noundef %70, ptr noundef %73) #8, !dbg !1408
  br label %83, !dbg !1409

75:                                               ; preds = %7
  %76 = load ptr, ptr @stderr, align 8, !dbg !1410
  %77 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1412
  %78 = load ptr, ptr %2, align 8, !dbg !1413
  %79 = getelementptr inbounds nuw %struct._kgflags_error, ptr %78, i32 0, i32 0, !dbg !1414
  %80 = load ptr, ptr %79, align 8, !dbg !1414
  %81 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %76, ptr noundef @.str.11, ptr noundef %77, ptr noundef %80) #8, !dbg !1415
  br label %83, !dbg !1416

82:                                               ; preds = %7
  br label %83, !dbg !1417

83:                                               ; preds = %82, %75, %68, %61, %58, %55, %45, %35, %28, %21, %14
  br label %84, !dbg !1418

84:                                               ; preds = %83
  %85 = load i32, ptr %1, align 4, !dbg !1419
  %86 = add nsw i32 %85, 1, !dbg !1419
  store i32 %86, ptr %1, align 4, !dbg !1419
  br label %3, !dbg !1420, !llvm.loop !1421

87:                                               ; preds = %3
  ret void, !dbg !1423
}

; Function Attrs: nounwind
declare i32 @fprintf(ptr noundef, ptr noundef, ...) #3

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @kgflags_print_usage() #0 !dbg !1424 {
  %1 = alloca i32, align 4
  %2 = alloca ptr, align 8
  %3 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 10), align 8, !dbg !1425
  %4 = icmp eq ptr %3, null, !dbg !1427
  br i1 %4, label %5, label %11, !dbg !1427

5:                                                ; preds = %0
  %6 = load ptr, ptr @stderr, align 8, !dbg !1428
  %7 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 9), align 8, !dbg !1430
  %8 = getelementptr inbounds ptr, ptr %7, i64 0, !dbg !1431
  %9 = load ptr, ptr %8, align 8, !dbg !1431
  %10 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %6, ptr noundef @.str.12, ptr noundef %9) #8, !dbg !1432
  br label %15, !dbg !1433

11:                                               ; preds = %0
  %12 = load ptr, ptr @stderr, align 8, !dbg !1434
  %13 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 10), align 8, !dbg !1436
  %14 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %12, ptr noundef @.str.13, ptr noundef %13) #8, !dbg !1437
  br label %15

15:                                               ; preds = %11, %5
  %16 = load ptr, ptr @stderr, align 8, !dbg !1438
  %17 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %16, ptr noundef @.str.14) #8, !dbg !1439
    #dbg_declare(ptr %1, !1440, !DIExpression(), !1442)
  store i32 0, ptr %1, align 4, !dbg !1442
  br label %18, !dbg !1443

18:                                               ; preds = %186, %15
  %19 = load i32, ptr %1, align 4, !dbg !1444
  %20 = load i32, ptr @_kgflags_g, align 8, !dbg !1446
  %21 = icmp slt i32 %19, %20, !dbg !1447
  br i1 %21, label %22, label %189, !dbg !1448

22:                                               ; preds = %18
    #dbg_declare(ptr %2, !1449, !DIExpression(), !1451)
  %23 = load i32, ptr %1, align 4, !dbg !1452
  %24 = sext i32 %23 to i64, !dbg !1453
  %25 = getelementptr inbounds [256 x %struct._kgflags_flag], ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 1), i64 0, i64 %24, !dbg !1453
  store ptr %25, ptr %2, align 8, !dbg !1451
  %26 = load ptr, ptr %2, align 8, !dbg !1454
  %27 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %26, i32 0, i32 7, !dbg !1455
  %28 = load i32, ptr %27, align 4, !dbg !1455
  switch i32 %28, label %171 [
    i32 1, label %29
    i32 2, label %53
    i32 3, label %84
    i32 4, label %108
    i32 5, label %132
    i32 6, label %145
    i32 7, label %158
  ], !dbg !1456

29:                                               ; preds = %22
  %30 = load ptr, ptr @stderr, align 8, !dbg !1457
  %31 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1459
  %32 = load ptr, ptr %2, align 8, !dbg !1460
  %33 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %32, i32 0, i32 0, !dbg !1461
  %34 = load ptr, ptr %33, align 8, !dbg !1461
  %35 = load ptr, ptr %2, align 8, !dbg !1462
  %36 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %35, i32 0, i32 6, !dbg !1463
  %37 = load i8, ptr %36, align 2, !dbg !1463
  %38 = trunc i8 %37 to i1, !dbg !1463
  %39 = zext i1 %38 to i64, !dbg !1462
  %40 = select i1 %38, ptr @.str.16, ptr @.str.17, !dbg !1462
  %41 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %30, ptr noundef @.str.15, ptr noundef %31, ptr noundef %34, ptr noundef %40) #8, !dbg !1464
  %42 = load ptr, ptr %2, align 8, !dbg !1465
  %43 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %42, i32 0, i32 6, !dbg !1467
  %44 = load i8, ptr %43, align 2, !dbg !1467
  %45 = trunc i8 %44 to i1, !dbg !1467
  br i1 %45, label %52, label %46, !dbg !1468

46:                                               ; preds = %29
  %47 = load ptr, ptr @stderr, align 8, !dbg !1469
  %48 = load ptr, ptr %2, align 8, !dbg !1471
  %49 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %48, i32 0, i32 2, !dbg !1472
  %50 = load ptr, ptr %49, align 8, !dbg !1473
  %51 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %47, ptr noundef @.str.18, ptr noundef %50) #8, !dbg !1474
  br label %52, !dbg !1475

52:                                               ; preds = %46, %29
  br label %172, !dbg !1476

53:                                               ; preds = %22
  %54 = load ptr, ptr @stderr, align 8, !dbg !1477
  %55 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1479
  %56 = load ptr, ptr %2, align 8, !dbg !1480
  %57 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %56, i32 0, i32 0, !dbg !1481
  %58 = load ptr, ptr %57, align 8, !dbg !1481
  %59 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1482
  %60 = load ptr, ptr %2, align 8, !dbg !1483
  %61 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %60, i32 0, i32 0, !dbg !1484
  %62 = load ptr, ptr %61, align 8, !dbg !1484
  %63 = load ptr, ptr %2, align 8, !dbg !1485
  %64 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %63, i32 0, i32 6, !dbg !1486
  %65 = load i8, ptr %64, align 2, !dbg !1486
  %66 = trunc i8 %65 to i1, !dbg !1486
  %67 = zext i1 %66 to i64, !dbg !1485
  %68 = select i1 %66, ptr @.str.16, ptr @.str.17, !dbg !1485
  %69 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %54, ptr noundef @.str.19, ptr noundef %55, ptr noundef %58, ptr noundef %59, ptr noundef %62, ptr noundef %68) #8, !dbg !1487
  %70 = load ptr, ptr %2, align 8, !dbg !1488
  %71 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %70, i32 0, i32 6, !dbg !1490
  %72 = load i8, ptr %71, align 2, !dbg !1490
  %73 = trunc i8 %72 to i1, !dbg !1490
  br i1 %73, label %83, label %74, !dbg !1491

74:                                               ; preds = %53
  %75 = load ptr, ptr @stderr, align 8, !dbg !1492
  %76 = load ptr, ptr %2, align 8, !dbg !1494
  %77 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %76, i32 0, i32 2, !dbg !1495
  %78 = load i8, ptr %77, align 8, !dbg !1496
  %79 = trunc i8 %78 to i1, !dbg !1496
  %80 = zext i1 %79 to i64, !dbg !1494
  %81 = select i1 %79, ptr @.str.20, ptr @.str.21, !dbg !1494
  %82 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %75, ptr noundef @.str.18, ptr noundef %81) #8, !dbg !1497
  br label %83, !dbg !1498

83:                                               ; preds = %74, %53
  br label %172, !dbg !1499

84:                                               ; preds = %22
  %85 = load ptr, ptr @stderr, align 8, !dbg !1500
  %86 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1502
  %87 = load ptr, ptr %2, align 8, !dbg !1503
  %88 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %87, i32 0, i32 0, !dbg !1504
  %89 = load ptr, ptr %88, align 8, !dbg !1504
  %90 = load ptr, ptr %2, align 8, !dbg !1505
  %91 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %90, i32 0, i32 6, !dbg !1506
  %92 = load i8, ptr %91, align 2, !dbg !1506
  %93 = trunc i8 %92 to i1, !dbg !1506
  %94 = zext i1 %93 to i64, !dbg !1505
  %95 = select i1 %93, ptr @.str.16, ptr @.str.17, !dbg !1505
  %96 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %85, ptr noundef @.str.22, ptr noundef %86, ptr noundef %89, ptr noundef %95) #8, !dbg !1507
  %97 = load ptr, ptr %2, align 8, !dbg !1508
  %98 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %97, i32 0, i32 6, !dbg !1510
  %99 = load i8, ptr %98, align 2, !dbg !1510
  %100 = trunc i8 %99 to i1, !dbg !1510
  br i1 %100, label %107, label %101, !dbg !1511

101:                                              ; preds = %84
  %102 = load ptr, ptr @stderr, align 8, !dbg !1512
  %103 = load ptr, ptr %2, align 8, !dbg !1514
  %104 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %103, i32 0, i32 2, !dbg !1515
  %105 = load i32, ptr %104, align 8, !dbg !1516
  %106 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %102, ptr noundef @.str.23, i32 noundef %105) #8, !dbg !1517
  br label %107, !dbg !1518

107:                                              ; preds = %101, %84
  br label %172, !dbg !1519

108:                                              ; preds = %22
  %109 = load ptr, ptr @stderr, align 8, !dbg !1520
  %110 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1522
  %111 = load ptr, ptr %2, align 8, !dbg !1523
  %112 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %111, i32 0, i32 0, !dbg !1524
  %113 = load ptr, ptr %112, align 8, !dbg !1524
  %114 = load ptr, ptr %2, align 8, !dbg !1525
  %115 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %114, i32 0, i32 6, !dbg !1526
  %116 = load i8, ptr %115, align 2, !dbg !1526
  %117 = trunc i8 %116 to i1, !dbg !1526
  %118 = zext i1 %117 to i64, !dbg !1525
  %119 = select i1 %117, ptr @.str.16, ptr @.str.17, !dbg !1525
  %120 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %109, ptr noundef @.str.24, ptr noundef %110, ptr noundef %113, ptr noundef %119) #8, !dbg !1527
  %121 = load ptr, ptr %2, align 8, !dbg !1528
  %122 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %121, i32 0, i32 6, !dbg !1530
  %123 = load i8, ptr %122, align 2, !dbg !1530
  %124 = trunc i8 %123 to i1, !dbg !1530
  br i1 %124, label %131, label %125, !dbg !1531

125:                                              ; preds = %108
  %126 = load ptr, ptr @stderr, align 8, !dbg !1532
  %127 = load ptr, ptr %2, align 8, !dbg !1534
  %128 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %127, i32 0, i32 2, !dbg !1535
  %129 = load double, ptr %128, align 8, !dbg !1536
  %130 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %126, ptr noundef @.str.25, double noundef %129) #8, !dbg !1537
  br label %131, !dbg !1538

131:                                              ; preds = %125, %108
  br label %172, !dbg !1539

132:                                              ; preds = %22
  %133 = load ptr, ptr @stderr, align 8, !dbg !1540
  %134 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1542
  %135 = load ptr, ptr %2, align 8, !dbg !1543
  %136 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %135, i32 0, i32 0, !dbg !1544
  %137 = load ptr, ptr %136, align 8, !dbg !1544
  %138 = load ptr, ptr %2, align 8, !dbg !1545
  %139 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %138, i32 0, i32 6, !dbg !1546
  %140 = load i8, ptr %139, align 2, !dbg !1546
  %141 = trunc i8 %140 to i1, !dbg !1546
  %142 = zext i1 %141 to i64, !dbg !1545
  %143 = select i1 %141, ptr @.str.16, ptr @.str.17, !dbg !1545
  %144 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %133, ptr noundef @.str.26, ptr noundef %134, ptr noundef %137, ptr noundef %143) #8, !dbg !1547
  br label %172, !dbg !1548

145:                                              ; preds = %22
  %146 = load ptr, ptr @stderr, align 8, !dbg !1549
  %147 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1551
  %148 = load ptr, ptr %2, align 8, !dbg !1552
  %149 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %148, i32 0, i32 0, !dbg !1553
  %150 = load ptr, ptr %149, align 8, !dbg !1553
  %151 = load ptr, ptr %2, align 8, !dbg !1554
  %152 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %151, i32 0, i32 6, !dbg !1555
  %153 = load i8, ptr %152, align 2, !dbg !1555
  %154 = trunc i8 %153 to i1, !dbg !1555
  %155 = zext i1 %154 to i64, !dbg !1554
  %156 = select i1 %154, ptr @.str.16, ptr @.str.17, !dbg !1554
  %157 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %146, ptr noundef @.str.27, ptr noundef %147, ptr noundef %150, ptr noundef %156) #8, !dbg !1556
  br label %172, !dbg !1557

158:                                              ; preds = %22
  %159 = load ptr, ptr @stderr, align 8, !dbg !1558
  %160 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 6), align 8, !dbg !1560
  %161 = load ptr, ptr %2, align 8, !dbg !1561
  %162 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %161, i32 0, i32 0, !dbg !1562
  %163 = load ptr, ptr %162, align 8, !dbg !1562
  %164 = load ptr, ptr %2, align 8, !dbg !1563
  %165 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %164, i32 0, i32 6, !dbg !1564
  %166 = load i8, ptr %165, align 2, !dbg !1564
  %167 = trunc i8 %166 to i1, !dbg !1564
  %168 = zext i1 %167 to i64, !dbg !1563
  %169 = select i1 %167, ptr @.str.16, ptr @.str.17, !dbg !1563
  %170 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %159, ptr noundef @.str.28, ptr noundef %160, ptr noundef %163, ptr noundef %169) #8, !dbg !1565
  br label %172, !dbg !1566

171:                                              ; preds = %22
  br label %172, !dbg !1567

172:                                              ; preds = %171, %158, %145, %132, %131, %107, %83, %52
  %173 = load ptr, ptr %2, align 8, !dbg !1568
  %174 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %173, i32 0, i32 1, !dbg !1570
  %175 = load ptr, ptr %174, align 8, !dbg !1570
  %176 = icmp ne ptr %175, null, !dbg !1568
  br i1 %176, label %177, label %183, !dbg !1568

177:                                              ; preds = %172
  %178 = load ptr, ptr @stderr, align 8, !dbg !1571
  %179 = load ptr, ptr %2, align 8, !dbg !1573
  %180 = getelementptr inbounds nuw %struct._kgflags_flag, ptr %179, i32 0, i32 1, !dbg !1574
  %181 = load ptr, ptr %180, align 8, !dbg !1574
  %182 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %178, ptr noundef @.str.29, ptr noundef %181) #8, !dbg !1575
  br label %183, !dbg !1576

183:                                              ; preds = %177, %172
  %184 = load ptr, ptr @stderr, align 8, !dbg !1577
  %185 = call i32 (ptr, ptr, ...) @fprintf(ptr noundef %184, ptr noundef @.str.30) #8, !dbg !1578
  br label %186, !dbg !1579

186:                                              ; preds = %183
  %187 = load i32, ptr %1, align 4, !dbg !1580
  %188 = add nsw i32 %187, 1, !dbg !1580
  store i32 %188, ptr %1, align 4, !dbg !1580
  br label %18, !dbg !1581, !llvm.loop !1582

189:                                              ; preds = %18
  ret void, !dbg !1584
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @kgflags_set_custom_description(ptr noundef %0) #0 !dbg !1585 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !1586, !DIExpression(), !1587)
  %3 = load ptr, ptr %2, align 8, !dbg !1588
  store ptr %3, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 10), align 8, !dbg !1589
  ret void, !dbg !1590
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @kgflags_string_array_get_count(ptr noundef %0) #0 !dbg !1591 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !1596, !DIExpression(), !1597)
  %3 = load ptr, ptr %2, align 8, !dbg !1598
  %4 = getelementptr inbounds nuw %struct.kgflags_string_array, ptr %3, i32 0, i32 1, !dbg !1599
  %5 = load i32, ptr %4, align 8, !dbg !1599
  ret i32 %5, !dbg !1600
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local ptr @kgflags_string_array_get_item(ptr noundef %0, i32 noundef %1) #0 !dbg !1601 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
    #dbg_declare(ptr %4, !1604, !DIExpression(), !1605)
  store i32 %1, ptr %5, align 4
    #dbg_declare(ptr %5, !1606, !DIExpression(), !1607)
  %6 = load i32, ptr %5, align 4, !dbg !1608
  %7 = icmp slt i32 %6, 0, !dbg !1610
  br i1 %7, label %14, label %8, !dbg !1611

8:                                                ; preds = %2
  %9 = load i32, ptr %5, align 4, !dbg !1612
  %10 = load ptr, ptr %4, align 8, !dbg !1613
  %11 = getelementptr inbounds nuw %struct.kgflags_string_array, ptr %10, i32 0, i32 1, !dbg !1614
  %12 = load i32, ptr %11, align 8, !dbg !1614
  %13 = icmp sge i32 %9, %12, !dbg !1615
  br i1 %13, label %14, label %15, !dbg !1611

14:                                               ; preds = %8, %2
  store ptr null, ptr %3, align 8, !dbg !1616
  br label %23, !dbg !1616

15:                                               ; preds = %8
  %16 = load ptr, ptr %4, align 8, !dbg !1618
  %17 = getelementptr inbounds nuw %struct.kgflags_string_array, ptr %16, i32 0, i32 0, !dbg !1619
  %18 = load ptr, ptr %17, align 8, !dbg !1619
  %19 = load i32, ptr %5, align 4, !dbg !1620
  %20 = sext i32 %19 to i64, !dbg !1618
  %21 = getelementptr inbounds ptr, ptr %18, i64 %20, !dbg !1618
  %22 = load ptr, ptr %21, align 8, !dbg !1618
  store ptr %22, ptr %3, align 8, !dbg !1621
  br label %23, !dbg !1621

23:                                               ; preds = %15, %14
  %24 = load ptr, ptr %3, align 8, !dbg !1622
  ret ptr %24, !dbg !1622
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @kgflags_int_array_get_count(ptr noundef %0) #0 !dbg !1623 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !1628, !DIExpression(), !1629)
  %3 = load ptr, ptr %2, align 8, !dbg !1630
  %4 = getelementptr inbounds nuw %struct.kgflags_int_array, ptr %3, i32 0, i32 1, !dbg !1631
  %5 = load i32, ptr %4, align 8, !dbg !1631
  ret i32 %5, !dbg !1632
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @kgflags_int_array_get_item(ptr noundef %0, i32 noundef %1) #0 !dbg !1633 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca ptr, align 8
  %7 = alloca i8, align 1
  %8 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
    #dbg_declare(ptr %4, !1636, !DIExpression(), !1637)
  store i32 %1, ptr %5, align 4
    #dbg_declare(ptr %5, !1638, !DIExpression(), !1639)
  %9 = load i32, ptr %5, align 4, !dbg !1640
  %10 = icmp slt i32 %9, 0, !dbg !1642
  br i1 %10, label %17, label %11, !dbg !1643

11:                                               ; preds = %2
  %12 = load i32, ptr %5, align 4, !dbg !1644
  %13 = load ptr, ptr %4, align 8, !dbg !1645
  %14 = getelementptr inbounds nuw %struct.kgflags_int_array, ptr %13, i32 0, i32 1, !dbg !1646
  %15 = load i32, ptr %14, align 8, !dbg !1646
  %16 = icmp sge i32 %12, %15, !dbg !1647
  br i1 %16, label %17, label %18, !dbg !1643

17:                                               ; preds = %11, %2
  store i32 0, ptr %3, align 4, !dbg !1648
  br label %33, !dbg !1648

18:                                               ; preds = %11
    #dbg_declare(ptr %6, !1650, !DIExpression(), !1651)
  %19 = load ptr, ptr %4, align 8, !dbg !1652
  %20 = getelementptr inbounds nuw %struct.kgflags_int_array, ptr %19, i32 0, i32 0, !dbg !1653
  %21 = load ptr, ptr %20, align 8, !dbg !1653
  %22 = load i32, ptr %5, align 4, !dbg !1654
  %23 = sext i32 %22 to i64, !dbg !1652
  %24 = getelementptr inbounds ptr, ptr %21, i64 %23, !dbg !1652
  %25 = load ptr, ptr %24, align 8, !dbg !1652
  store ptr %25, ptr %6, align 8, !dbg !1651
    #dbg_declare(ptr %7, !1655, !DIExpression(), !1656)
  store i8 0, ptr %7, align 1, !dbg !1656
    #dbg_declare(ptr %8, !1657, !DIExpression(), !1658)
  %26 = load ptr, ptr %6, align 8, !dbg !1659
  %27 = call i32 @_kgflags_parse_int(ptr noundef %26, ptr noundef %7), !dbg !1660
  store i32 %27, ptr %8, align 4, !dbg !1658
  %28 = load i8, ptr %7, align 1, !dbg !1661
  %29 = trunc i8 %28 to i1, !dbg !1661
  br i1 %29, label %31, label %30, !dbg !1663

30:                                               ; preds = %18
  store i32 0, ptr %3, align 4, !dbg !1664
  br label %33, !dbg !1664

31:                                               ; preds = %18
  %32 = load i32, ptr %8, align 4, !dbg !1666
  store i32 %32, ptr %3, align 4, !dbg !1667
  br label %33, !dbg !1667

33:                                               ; preds = %31, %30, %17
  %34 = load i32, ptr %3, align 4, !dbg !1668
  ret i32 %34, !dbg !1668
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal i32 @_kgflags_parse_int(ptr noundef %0, ptr noundef %1) #0 !dbg !1669 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i64, align 8
  store ptr %0, ptr %4, align 8
    #dbg_declare(ptr %4, !1672, !DIExpression(), !1673)
  store ptr %1, ptr %5, align 8
    #dbg_declare(ptr %5, !1674, !DIExpression(), !1675)
  %8 = load ptr, ptr %5, align 8, !dbg !1676
  store i8 0, ptr %8, align 1, !dbg !1677
    #dbg_declare(ptr %6, !1678, !DIExpression(), !1679)
  store ptr null, ptr %6, align 8, !dbg !1679
    #dbg_declare(ptr %7, !1680, !DIExpression(), !1682)
  %9 = load ptr, ptr %4, align 8, !dbg !1683
  %10 = call i64 @strtol(ptr noundef %9, ptr noundef %6, i32 noundef 10) #8, !dbg !1684
  store i64 %10, ptr %7, align 8, !dbg !1682
  %11 = load ptr, ptr %6, align 8, !dbg !1685
  %12 = load ptr, ptr %4, align 8, !dbg !1687
  %13 = icmp eq ptr %11, %12, !dbg !1688
  br i1 %13, label %35, label %14, !dbg !1689

14:                                               ; preds = %2
  %15 = load ptr, ptr %6, align 8, !dbg !1690
  %16 = load i8, ptr %15, align 1, !dbg !1691
  %17 = sext i8 %16 to i32, !dbg !1691
  %18 = icmp ne i32 %17, 0, !dbg !1692
  br i1 %18, label %35, label %19, !dbg !1693

19:                                               ; preds = %14
  %20 = load i64, ptr %7, align 8, !dbg !1694
  %21 = icmp sgt i64 %20, 2147483647, !dbg !1695
  br i1 %21, label %35, label %22, !dbg !1696

22:                                               ; preds = %19
  %23 = load i64, ptr %7, align 8, !dbg !1697
  %24 = icmp slt i64 %23, -2147483648, !dbg !1698
  br i1 %24, label %35, label %25, !dbg !1699

25:                                               ; preds = %22
  %26 = load i64, ptr %7, align 8, !dbg !1700
  %27 = icmp eq i64 %26, -9223372036854775808, !dbg !1701
  br i1 %27, label %31, label %28, !dbg !1702

28:                                               ; preds = %25
  %29 = load i64, ptr %7, align 8, !dbg !1703
  %30 = icmp eq i64 %29, 9223372036854775807, !dbg !1704
  br i1 %30, label %31, label %37, !dbg !1705

31:                                               ; preds = %28, %25
  %32 = call ptr @__errno_location() #9, !dbg !1706
  %33 = load i32, ptr %32, align 4, !dbg !1706
  %34 = icmp eq i32 34, %33, !dbg !1707
  br i1 %34, label %35, label %37, !dbg !1699

35:                                               ; preds = %31, %22, %19, %14, %2
  %36 = load ptr, ptr %5, align 8, !dbg !1708
  store i8 0, ptr %36, align 1, !dbg !1710
  store i32 0, ptr %3, align 4, !dbg !1711
  br label %41, !dbg !1711

37:                                               ; preds = %31, %28
  %38 = load ptr, ptr %5, align 8, !dbg !1712
  store i8 1, ptr %38, align 1, !dbg !1713
  %39 = load i64, ptr %7, align 8, !dbg !1714
  %40 = trunc i64 %39 to i32, !dbg !1715
  store i32 %40, ptr %3, align 4, !dbg !1716
  br label %41, !dbg !1716

41:                                               ; preds = %37, %35
  %42 = load i32, ptr %3, align 4, !dbg !1717
  ret i32 %42, !dbg !1717
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @kgflags_double_array_get_count(ptr noundef %0) #0 !dbg !1718 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !1723, !DIExpression(), !1724)
  %3 = load ptr, ptr %2, align 8, !dbg !1725
  %4 = getelementptr inbounds nuw %struct.kgflags_double_array, ptr %3, i32 0, i32 1, !dbg !1726
  %5 = load i32, ptr %4, align 8, !dbg !1726
  ret i32 %5, !dbg !1727
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local double @kgflags_double_array_get_item(ptr noundef %0, i32 noundef %1) #0 !dbg !1728 {
  %3 = alloca double, align 8
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca ptr, align 8
  %7 = alloca i8, align 1
  %8 = alloca double, align 8
  store ptr %0, ptr %4, align 8
    #dbg_declare(ptr %4, !1731, !DIExpression(), !1732)
  store i32 %1, ptr %5, align 4
    #dbg_declare(ptr %5, !1733, !DIExpression(), !1734)
  %9 = load i32, ptr %5, align 4, !dbg !1735
  %10 = icmp slt i32 %9, 0, !dbg !1737
  br i1 %10, label %17, label %11, !dbg !1738

11:                                               ; preds = %2
  %12 = load i32, ptr %5, align 4, !dbg !1739
  %13 = load ptr, ptr %4, align 8, !dbg !1740
  %14 = getelementptr inbounds nuw %struct.kgflags_double_array, ptr %13, i32 0, i32 1, !dbg !1741
  %15 = load i32, ptr %14, align 8, !dbg !1741
  %16 = icmp sge i32 %12, %15, !dbg !1742
  br i1 %16, label %17, label %18, !dbg !1738

17:                                               ; preds = %11, %2
  store double 0.000000e+00, ptr %3, align 8, !dbg !1743
  br label %33, !dbg !1743

18:                                               ; preds = %11
    #dbg_declare(ptr %6, !1745, !DIExpression(), !1746)
  %19 = load ptr, ptr %4, align 8, !dbg !1747
  %20 = getelementptr inbounds nuw %struct.kgflags_double_array, ptr %19, i32 0, i32 0, !dbg !1748
  %21 = load ptr, ptr %20, align 8, !dbg !1748
  %22 = load i32, ptr %5, align 4, !dbg !1749
  %23 = sext i32 %22 to i64, !dbg !1747
  %24 = getelementptr inbounds ptr, ptr %21, i64 %23, !dbg !1747
  %25 = load ptr, ptr %24, align 8, !dbg !1747
  store ptr %25, ptr %6, align 8, !dbg !1746
    #dbg_declare(ptr %7, !1750, !DIExpression(), !1751)
  store i8 0, ptr %7, align 1, !dbg !1751
    #dbg_declare(ptr %8, !1752, !DIExpression(), !1753)
  %26 = load ptr, ptr %6, align 8, !dbg !1754
  %27 = call double @_kgflags_parse_double(ptr noundef %26, ptr noundef %7), !dbg !1755
  store double %27, ptr %8, align 8, !dbg !1753
  %28 = load i8, ptr %7, align 1, !dbg !1756
  %29 = trunc i8 %28 to i1, !dbg !1756
  br i1 %29, label %31, label %30, !dbg !1758

30:                                               ; preds = %18
  store double 0.000000e+00, ptr %3, align 8, !dbg !1759
  br label %33, !dbg !1759

31:                                               ; preds = %18
  %32 = load double, ptr %8, align 8, !dbg !1761
  store double %32, ptr %3, align 8, !dbg !1762
  br label %33, !dbg !1762

33:                                               ; preds = %31, %30, %17
  %34 = load double, ptr %3, align 8, !dbg !1763
  ret double %34, !dbg !1763
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal double @_kgflags_parse_double(ptr noundef %0, ptr noundef %1) #0 !dbg !1764 {
  %3 = alloca double, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca double, align 8
  store ptr %0, ptr %4, align 8
    #dbg_declare(ptr %4, !1767, !DIExpression(), !1768)
  store ptr %1, ptr %5, align 8
    #dbg_declare(ptr %5, !1769, !DIExpression(), !1770)
  %8 = load ptr, ptr %5, align 8, !dbg !1771
  store i8 0, ptr %8, align 1, !dbg !1772
    #dbg_declare(ptr %6, !1773, !DIExpression(), !1774)
  store ptr null, ptr %6, align 8, !dbg !1774
    #dbg_declare(ptr %7, !1775, !DIExpression(), !1776)
  %9 = load ptr, ptr %4, align 8, !dbg !1777
  %10 = call double @strtod(ptr noundef %9, ptr noundef %6) #8, !dbg !1778
  store double %10, ptr %7, align 8, !dbg !1776
  %11 = load ptr, ptr %6, align 8, !dbg !1779
  %12 = load ptr, ptr %4, align 8, !dbg !1781
  %13 = icmp eq ptr %11, %12, !dbg !1782
  br i1 %13, label %29, label %14, !dbg !1783

14:                                               ; preds = %2
  %15 = load ptr, ptr %6, align 8, !dbg !1784
  %16 = load i8, ptr %15, align 1, !dbg !1785
  %17 = sext i8 %16 to i32, !dbg !1785
  %18 = icmp ne i32 %17, 0, !dbg !1786
  br i1 %18, label %29, label %19, !dbg !1787

19:                                               ; preds = %14
  %20 = load double, ptr %7, align 8, !dbg !1788
  %21 = fcmp oeq double %20, 0xFFF0000000000000, !dbg !1789
  br i1 %21, label %25, label %22, !dbg !1790

22:                                               ; preds = %19
  %23 = load double, ptr %7, align 8, !dbg !1791
  %24 = fcmp oeq double %23, 0x7FF0000000000000, !dbg !1792
  br i1 %24, label %25, label %31, !dbg !1793

25:                                               ; preds = %22, %19
  %26 = call ptr @__errno_location() #9, !dbg !1794
  %27 = load i32, ptr %26, align 4, !dbg !1794
  %28 = icmp eq i32 34, %27, !dbg !1795
  br i1 %28, label %29, label %31, !dbg !1787

29:                                               ; preds = %25, %14, %2
  %30 = load ptr, ptr %5, align 8, !dbg !1796
  store i8 0, ptr %30, align 1, !dbg !1798
  store double 0.000000e+00, ptr %3, align 8, !dbg !1799
  br label %34, !dbg !1799

31:                                               ; preds = %25, %22
  %32 = load ptr, ptr %5, align 8, !dbg !1800
  store i8 1, ptr %32, align 1, !dbg !1801
  %33 = load double, ptr %7, align 8, !dbg !1802
  store double %33, ptr %3, align 8, !dbg !1803
  br label %34, !dbg !1803

34:                                               ; preds = %31, %29
  %35 = load double, ptr %3, align 8, !dbg !1804
  ret double %35, !dbg !1804
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @kgflags_get_non_flag_args_count() #0 !dbg !1805 {
  %1 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 2), align 8, !dbg !1808
  ret i32 %1, !dbg !1809
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local ptr @kgflags_get_non_flag_arg(i32 noundef %0) #0 !dbg !1810 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
    #dbg_declare(ptr %3, !1813, !DIExpression(), !1814)
  %4 = load i32, ptr %3, align 4, !dbg !1815
  %5 = icmp slt i32 %4, 0, !dbg !1817
  br i1 %5, label %10, label %6, !dbg !1818

6:                                                ; preds = %1
  %7 = load i32, ptr %3, align 4, !dbg !1819
  %8 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 2), align 8, !dbg !1820
  %9 = icmp sge i32 %7, %8, !dbg !1821
  br i1 %9, label %10, label %11, !dbg !1818

10:                                               ; preds = %6, %1
  store ptr null, ptr %2, align 8, !dbg !1822
  br label %16, !dbg !1822

11:                                               ; preds = %6
  %12 = load i32, ptr %3, align 4, !dbg !1824
  %13 = sext i32 %12 to i64, !dbg !1825
  %14 = getelementptr inbounds [512 x ptr], ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 3), i64 0, i64 %13, !dbg !1825
  %15 = load ptr, ptr %14, align 8, !dbg !1825
  store ptr %15, ptr %2, align 8, !dbg !1826
  br label %16, !dbg !1826

16:                                               ; preds = %11, %10
  %17 = load ptr, ptr %2, align 8, !dbg !1827
  ret ptr %17, !dbg !1827
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @main(i32 noundef %0, ptr noundef %1) #0 !dbg !1828 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i8, align 1
  %8 = alloca i32, align 4
  %9 = alloca double, align 8
  %10 = alloca %struct.kgflags_string_array, align 8
  %11 = alloca %struct.kgflags_int_array, align 8
  %12 = alloca %struct.kgflags_double_array, align 8
  %13 = alloca i32, align 4
  %14 = alloca i32, align 4
  %15 = alloca i32, align 4
  %16 = alloca i32, align 4
  %17 = alloca i32, align 4
  store i32 0, ptr %3, align 4
  store i32 %0, ptr %4, align 4
    #dbg_declare(ptr %4, !1831, !DIExpression(), !1832)
  store ptr %1, ptr %5, align 8
    #dbg_declare(ptr %5, !1833, !DIExpression(), !1834)
    #dbg_declare(ptr %6, !1835, !DIExpression(), !1836)
  store ptr null, ptr %6, align 8, !dbg !1836
  call void @kgflags_string(ptr noundef @.str.31, ptr noundef @.str.32, ptr noundef @.str.33, i1 noundef zeroext true, ptr noundef %6), !dbg !1837
    #dbg_declare(ptr %7, !1838, !DIExpression(), !1839)
  store i8 0, ptr %7, align 1, !dbg !1839
  call void @kgflags_bool(ptr noundef @.str.34, i1 noundef zeroext false, ptr noundef @.str.35, i1 noundef zeroext true, ptr noundef %7), !dbg !1840
    #dbg_declare(ptr %8, !1841, !DIExpression(), !1842)
  store i32 0, ptr %8, align 4, !dbg !1842
  call void @kgflags_int(ptr noundef @.str.36, i32 noundef 0, ptr noundef @.str.37, i1 noundef zeroext true, ptr noundef %8), !dbg !1843
    #dbg_declare(ptr %9, !1844, !DIExpression(), !1845)
  store double 0.000000e+00, ptr %9, align 8, !dbg !1845
  call void @kgflags_double(ptr noundef @.str.38, double noundef 0.000000e+00, ptr noundef @.str.39, i1 noundef zeroext true, ptr noundef %9), !dbg !1846
    #dbg_declare(ptr %10, !1847, !DIExpression(), !1848)
  call void @kgflags_string_array(ptr noundef @.str.40, ptr noundef @.str.41, i1 noundef zeroext true, ptr noundef %10), !dbg !1849
    #dbg_declare(ptr %11, !1850, !DIExpression(), !1851)
  call void @kgflags_int_array(ptr noundef @.str.42, ptr noundef @.str.43, i1 noundef zeroext true, ptr noundef %11), !dbg !1852
    #dbg_declare(ptr %12, !1853, !DIExpression(), !1854)
  call void @kgflags_double_array(ptr noundef @.str.44, ptr noundef @.str.45, i1 noundef zeroext true, ptr noundef %12), !dbg !1855
  call void @kgflags_set_custom_description(ptr noundef @.str.46), !dbg !1856
  call void @kgflags_set_prefix(ptr noundef @.str.47), !dbg !1857
  %18 = load i32, ptr %4, align 4, !dbg !1858
  %19 = load ptr, ptr %5, align 8, !dbg !1860
  %20 = call zeroext i1 @kgflags_parse(i32 noundef %18, ptr noundef %19), !dbg !1861
  br i1 %20, label %22, label %21, !dbg !1862

21:                                               ; preds = %2
  call void @kgflags_print_errors(), !dbg !1863
  call void @kgflags_print_usage(), !dbg !1865
  store i32 1, ptr %3, align 4, !dbg !1866
  br label %93, !dbg !1866

22:                                               ; preds = %2
  %23 = load ptr, ptr %6, align 8, !dbg !1867
  %24 = call i32 (ptr, ...) @printf(ptr noundef @.str.48, ptr noundef %23), !dbg !1868
  %25 = load i8, ptr %7, align 1, !dbg !1869
  %26 = trunc i8 %25 to i1, !dbg !1869
  %27 = zext i1 %26 to i64, !dbg !1869
  %28 = select i1 %26, ptr @.str.50, ptr @.str.51, !dbg !1869
  %29 = call i32 (ptr, ...) @printf(ptr noundef @.str.49, ptr noundef %28), !dbg !1870
  %30 = load i32, ptr %8, align 4, !dbg !1871
  %31 = call i32 (ptr, ...) @printf(ptr noundef @.str.52, i32 noundef %30), !dbg !1872
  %32 = load double, ptr %9, align 8, !dbg !1873
  %33 = call i32 (ptr, ...) @printf(ptr noundef @.str.53, double noundef %32), !dbg !1874
  %34 = call i32 @kgflags_string_array_get_count(ptr noundef %10), !dbg !1875
  %35 = call i32 (ptr, ...) @printf(ptr noundef @.str.54, i32 noundef %34), !dbg !1876
    #dbg_declare(ptr %13, !1877, !DIExpression(), !1879)
  store i32 0, ptr %13, align 4, !dbg !1879
  br label %36, !dbg !1880

36:                                               ; preds = %45, %22
  %37 = load i32, ptr %13, align 4, !dbg !1881
  %38 = call i32 @kgflags_string_array_get_count(ptr noundef %10), !dbg !1883
  %39 = icmp slt i32 %37, %38, !dbg !1884
  br i1 %39, label %40, label %48, !dbg !1885

40:                                               ; preds = %36
  %41 = load i32, ptr %13, align 4, !dbg !1886
  %42 = load i32, ptr %13, align 4, !dbg !1888
  %43 = call ptr @kgflags_string_array_get_item(ptr noundef %10, i32 noundef %42), !dbg !1889
  %44 = call i32 (ptr, ...) @printf(ptr noundef @.str.55, i32 noundef %41, ptr noundef %43), !dbg !1890
  br label %45, !dbg !1891

45:                                               ; preds = %40
  %46 = load i32, ptr %13, align 4, !dbg !1892
  %47 = add nsw i32 %46, 1, !dbg !1892
  store i32 %47, ptr %13, align 4, !dbg !1892
  br label %36, !dbg !1893, !llvm.loop !1894

48:                                               ; preds = %36
  %49 = call i32 @kgflags_int_array_get_count(ptr noundef %11), !dbg !1896
  %50 = call i32 (ptr, ...) @printf(ptr noundef @.str.56, i32 noundef %49), !dbg !1897
    #dbg_declare(ptr %14, !1898, !DIExpression(), !1900)
  store i32 0, ptr %14, align 4, !dbg !1900
  br label %51, !dbg !1901

51:                                               ; preds = %60, %48
  %52 = load i32, ptr %14, align 4, !dbg !1902
  %53 = call i32 @kgflags_int_array_get_count(ptr noundef %11), !dbg !1904
  %54 = icmp slt i32 %52, %53, !dbg !1905
  br i1 %54, label %55, label %63, !dbg !1906

55:                                               ; preds = %51
  %56 = load i32, ptr %14, align 4, !dbg !1907
  %57 = load i32, ptr %14, align 4, !dbg !1909
  %58 = call i32 @kgflags_int_array_get_item(ptr noundef %11, i32 noundef %57), !dbg !1910
  %59 = call i32 (ptr, ...) @printf(ptr noundef @.str.57, i32 noundef %56, i32 noundef %58), !dbg !1911
  br label %60, !dbg !1912

60:                                               ; preds = %55
  %61 = load i32, ptr %14, align 4, !dbg !1913
  %62 = add nsw i32 %61, 1, !dbg !1913
  store i32 %62, ptr %14, align 4, !dbg !1913
  br label %51, !dbg !1914, !llvm.loop !1915

63:                                               ; preds = %51
  %64 = call i32 @kgflags_double_array_get_count(ptr noundef %12), !dbg !1917
  %65 = call i32 (ptr, ...) @printf(ptr noundef @.str.58, i32 noundef %64), !dbg !1918
    #dbg_declare(ptr %15, !1919, !DIExpression(), !1921)
  store i32 0, ptr %15, align 4, !dbg !1921
  br label %66, !dbg !1922

66:                                               ; preds = %75, %63
  %67 = load i32, ptr %15, align 4, !dbg !1923
  %68 = call i32 @kgflags_double_array_get_count(ptr noundef %12), !dbg !1925
  %69 = icmp slt i32 %67, %68, !dbg !1926
  br i1 %69, label %70, label %78, !dbg !1927

70:                                               ; preds = %66
  %71 = load i32, ptr %15, align 4, !dbg !1928
  %72 = load i32, ptr %15, align 4, !dbg !1930
  %73 = call double @kgflags_double_array_get_item(ptr noundef %12, i32 noundef %72), !dbg !1931
  %74 = call i32 (ptr, ...) @printf(ptr noundef @.str.59, i32 noundef %71, double noundef %73), !dbg !1932
  br label %75, !dbg !1933

75:                                               ; preds = %70
  %76 = load i32, ptr %15, align 4, !dbg !1934
  %77 = add nsw i32 %76, 1, !dbg !1934
  store i32 %77, ptr %15, align 4, !dbg !1934
  br label %66, !dbg !1935, !llvm.loop !1936

78:                                               ; preds = %66
    #dbg_declare(ptr %16, !1938, !DIExpression(), !1939)
  %79 = call i32 @kgflags_get_non_flag_args_count(), !dbg !1940
  store i32 %79, ptr %16, align 4, !dbg !1939
    #dbg_declare(ptr %17, !1941, !DIExpression(), !1943)
  store i32 0, ptr %17, align 4, !dbg !1943
  br label %80, !dbg !1944

80:                                               ; preds = %89, %78
  %81 = load i32, ptr %17, align 4, !dbg !1945
  %82 = load i32, ptr %16, align 4, !dbg !1947
  %83 = icmp slt i32 %81, %82, !dbg !1948
  br i1 %83, label %84, label %92, !dbg !1949

84:                                               ; preds = %80
  %85 = load i32, ptr %17, align 4, !dbg !1950
  %86 = load i32, ptr %17, align 4, !dbg !1952
  %87 = call ptr @kgflags_get_non_flag_arg(i32 noundef %86), !dbg !1953
  %88 = call i32 (ptr, ...) @printf(ptr noundef @.str.60, i32 noundef %85, ptr noundef %87), !dbg !1954
  br label %89, !dbg !1955

89:                                               ; preds = %84
  %90 = load i32, ptr %17, align 4, !dbg !1956
  %91 = add nsw i32 %90, 1, !dbg !1956
  store i32 %91, ptr %17, align 4, !dbg !1956
  br label %80, !dbg !1957, !llvm.loop !1958

92:                                               ; preds = %80
  store i32 0, ptr %3, align 4, !dbg !1960
  br label %93, !dbg !1960

93:                                               ; preds = %92, %21
  %94 = load i32, ptr %3, align 4, !dbg !1961
  ret i32 %94, !dbg !1961
}

declare i32 @printf(ptr noundef, ...) #4

; Function Attrs: nounwind willreturn memory(read)
declare i64 @strlen(ptr noundef) #2

; Function Attrs: nounwind willreturn memory(read)
declare i32 @strncmp(ptr noundef, ptr noundef, i64 noundef) #2

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #5

; Function Attrs: nounwind willreturn memory(read)
declare i32 @strcmp(ptr noundef, ptr noundef) #2

; Function Attrs: nounwind
declare i64 @strtol(ptr noundef, ptr noundef, i32 noundef) #3

; Function Attrs: nounwind willreturn memory(none)
declare ptr @__errno_location() #6

; Function Attrs: nounwind
declare double @strtod(ptr noundef, ptr noundef) #3

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal ptr @_kgflags_peek_arg() #0 !dbg !1962 {
  %1 = alloca ptr, align 8
  %2 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 7), align 8, !dbg !1963
  %3 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 8), align 4, !dbg !1965
  %4 = icmp sge i32 %2, %3, !dbg !1966
  br i1 %4, label %5, label %6, !dbg !1966

5:                                                ; preds = %0
  store ptr null, ptr %1, align 8, !dbg !1967
  br label %12, !dbg !1967

6:                                                ; preds = %0
  %7 = load ptr, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 9), align 8, !dbg !1969
  %8 = load i32, ptr getelementptr inbounds nuw (%struct.anon, ptr @_kgflags_g, i32 0, i32 7), align 8, !dbg !1970
  %9 = sext i32 %8 to i64, !dbg !1971
  %10 = getelementptr inbounds ptr, ptr %7, i64 %9, !dbg !1971
  %11 = load ptr, ptr %10, align 8, !dbg !1971
  store ptr %11, ptr %1, align 8, !dbg !1972
  br label %12, !dbg !1972

12:                                               ; preds = %6, %5
  %13 = load ptr, ptr %1, align 8, !dbg !1973
  ret ptr %13, !dbg !1973
}

attributes #0 = { noinline nounwind optnone sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #2 = { nounwind willreturn memory(read) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #6 = { nounwind willreturn memory(none) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { nounwind willreturn memory(read) }
attributes #8 = { nounwind }
attributes #9 = { nounwind willreturn memory(none) }

!llvm.dbg.cu = !{!9}
!llvm.module.flags = !{!332, !333, !334, !335, !336, !337, !338}
!llvm.ident = !{!339}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(scope: null, file: !2, line: 244, type: !3, isLocal: true, isDefinition: true)
!2 = !DIFile(filename: "./kgflags.c", directory: "/home/sentenced/Documents/Work/UCL/Repos/InformationFlow/tests/third_party", checksumkind: CSK_MD5, checksum: "e1adb0e0578ffda21cffb5b4f95e60ef")
!3 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 32, elements: !5)
!4 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!5 = !{!6}
!6 = !DISubrange(count: 4)
!7 = !DIGlobalVariableExpression(var: !8, expr: !DIExpression())
!8 = distinct !DIGlobalVariable(name: "_kgflags_g", scope: !9, file: !2, line: 224, type: !250, isLocal: true, isDefinition: true)
!9 = distinct !DICompileUnit(language: DW_LANG_C11, file: !10, producer: "clang version 20.1.8", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !11, retainedTypes: !36, globals: !39, splitDebugInlining: false, nameTableKind: None)
!10 = !DIFile(filename: "kgflags.c", directory: "/home/sentenced/Documents/Work/UCL/Repos/InformationFlow/tests/third_party", checksumkind: CSK_MD5, checksum: "e1adb0e0578ffda21cffb5b4f95e60ef")
!11 = !{!12, !23}
!12 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "_kgflags_flag_kind", file: !2, line: 139, baseType: !13, size: 32, elements: !14)
!13 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!14 = !{!15, !16, !17, !18, !19, !20, !21, !22}
!15 = !DIEnumerator(name: "KGFLAGS_FLAG_KIND_NONE", value: 0)
!16 = !DIEnumerator(name: "KGFLAGS_FLAG_KIND_STRING", value: 1)
!17 = !DIEnumerator(name: "KGFLAGS_FLAG_KIND_BOOL", value: 2)
!18 = !DIEnumerator(name: "KGFLAGS_FLAG_KIND_INT", value: 3)
!19 = !DIEnumerator(name: "KGFLAGS_FLAG_KIND_DOUBLE", value: 4)
!20 = !DIEnumerator(name: "KGFLAGS_FLAG_KIND_STRING_ARRAY", value: 5)
!21 = !DIEnumerator(name: "KGFLAGS_FLAG_KIND_INT_ARRAY", value: 6)
!22 = !DIEnumerator(name: "KGFLAGS_FLAG_KIND_DOUBLE_ARRAY", value: 7)
!23 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "_kgflags_error_kind", file: !2, line: 174, baseType: !13, size: 32, elements: !24)
!24 = !{!25, !26, !27, !28, !29, !30, !31, !32, !33, !34, !35}
!25 = !DIEnumerator(name: "KGFLAGS_ERROR_KIND_NONE", value: 0)
!26 = !DIEnumerator(name: "KGFLAGS_ERROR_KIND_MISSING_VALUE", value: 1)
!27 = !DIEnumerator(name: "KGFLAGS_ERROR_KIND_UNKNOWN_FLAG", value: 2)
!28 = !DIEnumerator(name: "KGFLAGS_ERROR_KIND_UNASSIGNED_FLAG", value: 3)
!29 = !DIEnumerator(name: "KGFLAGS_ERROR_KIND_INVALID_INT", value: 4)
!30 = !DIEnumerator(name: "KGFLAGS_ERROR_KIND_INVALID_DOUBLE", value: 5)
!31 = !DIEnumerator(name: "KGFLAGS_ERROR_KIND_TOO_MANY_FLAGS", value: 6)
!32 = !DIEnumerator(name: "KGFLAGS_ERROR_KIND_TOO_MANY_NON_FLAG_ARGS", value: 7)
!33 = !DIEnumerator(name: "KGFLAGS_ERROR_KIND_MULTIPLE_ASSIGNMENT", value: 8)
!34 = !DIEnumerator(name: "KGFLAGS_ERROR_KIND_DUPLICATE_FLAG", value: 9)
!35 = !DIEnumerator(name: "KGFLAGS_ERROR_KIND_PREFIX_NO", value: 10)
!36 = !{!37, !38}
!37 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!38 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!39 = !{!0, !40, !45, !50, !55, !60, !65, !70, !72, !77, !82, !87, !92, !97, !99, !104, !109, !114, !119, !124, !129, !134, !139, !144, !146, !151, !153, !155, !160, !165, !167, !169, !174, !176, !181, !183, !185, !187, !189, !191, !193, !195, !200, !205, !207, !209, !211, !213, !215, !217, !219, !221, !223, !225, !227, !232, !237, !239, !241, !243, !248, !7}
!40 = !DIGlobalVariableExpression(var: !41, expr: !DIExpression())
!41 = distinct !DIGlobalVariable(scope: null, file: !2, line: 346, type: !42, isLocal: true, isDefinition: true)
!42 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 24, elements: !43)
!43 = !{!44}
!44 = !DISubrange(count: 3)
!45 = !DIGlobalVariableExpression(var: !46, expr: !DIExpression())
!46 = distinct !DIGlobalVariable(scope: null, file: !2, line: 398, type: !47, isLocal: true, isDefinition: true)
!47 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 240, elements: !48)
!48 = !{!49}
!49 = !DISubrange(count: 30)
!50 = !DIGlobalVariableExpression(var: !51, expr: !DIExpression())
!51 = distinct !DIGlobalVariable(scope: null, file: !2, line: 402, type: !52, isLocal: true, isDefinition: true)
!52 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 200, elements: !53)
!53 = !{!54}
!54 = !DISubrange(count: 25)
!55 = !DIGlobalVariableExpression(var: !56, expr: !DIExpression())
!56 = distinct !DIGlobalVariable(scope: null, file: !2, line: 406, type: !57, isLocal: true, isDefinition: true)
!57 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 256, elements: !58)
!58 = !{!59}
!59 = !DISubrange(count: 32)
!60 = !DIGlobalVariableExpression(var: !61, expr: !DIExpression())
!61 = distinct !DIGlobalVariable(scope: null, file: !2, line: 410, type: !62, isLocal: true, isDefinition: true)
!62 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 456, elements: !63)
!63 = !{!64}
!64 = !DISubrange(count: 57)
!65 = !DIGlobalVariableExpression(var: !66, expr: !DIExpression())
!66 = distinct !DIGlobalVariable(scope: null, file: !2, line: 414, type: !67, isLocal: true, isDefinition: true)
!67 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 448, elements: !68)
!68 = !{!69}
!69 = !DISubrange(count: 56)
!70 = !DIGlobalVariableExpression(var: !71, expr: !DIExpression())
!71 = distinct !DIGlobalVariable(scope: null, file: !2, line: 418, type: !52, isLocal: true, isDefinition: true)
!72 = !DIGlobalVariableExpression(var: !73, expr: !DIExpression())
!73 = distinct !DIGlobalVariable(scope: null, file: !2, line: 422, type: !74, isLocal: true, isDefinition: true)
!74 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 376, elements: !75)
!75 = !{!76}
!76 = !DISubrange(count: 47)
!77 = !DIGlobalVariableExpression(var: !78, expr: !DIExpression())
!78 = distinct !DIGlobalVariable(scope: null, file: !2, line: 426, type: !79, isLocal: true, isDefinition: true)
!79 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 280, elements: !80)
!80 = !{!81}
!81 = !DISubrange(count: 35)
!82 = !DIGlobalVariableExpression(var: !83, expr: !DIExpression())
!83 = distinct !DIGlobalVariable(scope: null, file: !2, line: 430, type: !84, isLocal: true, isDefinition: true)
!84 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 232, elements: !85)
!85 = !{!86}
!86 = !DISubrange(count: 29)
!87 = !DIGlobalVariableExpression(var: !88, expr: !DIExpression())
!88 = distinct !DIGlobalVariable(scope: null, file: !2, line: 434, type: !89, isLocal: true, isDefinition: true)
!89 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 424, elements: !90)
!90 = !{!91}
!91 = !DISubrange(count: 53)
!92 = !DIGlobalVariableExpression(var: !93, expr: !DIExpression())
!93 = distinct !DIGlobalVariable(scope: null, file: !2, line: 445, type: !94, isLocal: true, isDefinition: true)
!94 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 112, elements: !95)
!95 = !{!96}
!96 = !DISubrange(count: 14)
!97 = !DIGlobalVariableExpression(var: !98, expr: !DIExpression())
!98 = distinct !DIGlobalVariable(scope: null, file: !2, line: 447, type: !3, isLocal: true, isDefinition: true)
!99 = !DIGlobalVariableExpression(var: !100, expr: !DIExpression())
!100 = distinct !DIGlobalVariable(scope: null, file: !2, line: 450, type: !101, isLocal: true, isDefinition: true)
!101 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 64, elements: !102)
!102 = !{!103}
!103 = !DISubrange(count: 8)
!104 = !DIGlobalVariableExpression(var: !105, expr: !DIExpression())
!105 = distinct !DIGlobalVariable(scope: null, file: !2, line: 455, type: !106, isLocal: true, isDefinition: true)
!106 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 136, elements: !107)
!107 = !{!108}
!108 = !DISubrange(count: 17)
!109 = !DIGlobalVariableExpression(var: !110, expr: !DIExpression())
!110 = distinct !DIGlobalVariable(scope: null, file: !2, line: 455, type: !111, isLocal: true, isDefinition: true)
!111 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 16, elements: !112)
!112 = !{!113}
!113 = !DISubrange(count: 2)
!114 = !DIGlobalVariableExpression(var: !115, expr: !DIExpression())
!115 = distinct !DIGlobalVariable(scope: null, file: !2, line: 455, type: !116, isLocal: true, isDefinition: true)
!116 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 96, elements: !117)
!117 = !{!118}
!118 = !DISubrange(count: 12)
!119 = !DIGlobalVariableExpression(var: !120, expr: !DIExpression())
!120 = distinct !DIGlobalVariable(scope: null, file: !2, line: 457, type: !121, isLocal: true, isDefinition: true)
!121 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 120, elements: !122)
!122 = !{!123}
!123 = !DISubrange(count: 15)
!124 = !DIGlobalVariableExpression(var: !125, expr: !DIExpression())
!125 = distinct !DIGlobalVariable(scope: null, file: !2, line: 461, type: !126, isLocal: true, isDefinition: true)
!126 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 216, elements: !127)
!127 = !{!128}
!128 = !DISubrange(count: 27)
!129 = !DIGlobalVariableExpression(var: !130, expr: !DIExpression())
!130 = distinct !DIGlobalVariable(scope: null, file: !2, line: 464, type: !131, isLocal: true, isDefinition: true)
!131 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 40, elements: !132)
!132 = !{!133}
!133 = !DISubrange(count: 5)
!134 = !DIGlobalVariableExpression(var: !135, expr: !DIExpression())
!135 = distinct !DIGlobalVariable(scope: null, file: !2, line: 464, type: !136, isLocal: true, isDefinition: true)
!136 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 48, elements: !137)
!137 = !{!138}
!138 = !DISubrange(count: 6)
!139 = !DIGlobalVariableExpression(var: !140, expr: !DIExpression())
!140 = distinct !DIGlobalVariable(scope: null, file: !2, line: 469, type: !141, isLocal: true, isDefinition: true)
!141 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 144, elements: !142)
!142 = !{!143}
!143 = !DISubrange(count: 18)
!144 = !DIGlobalVariableExpression(var: !145, expr: !DIExpression())
!145 = distinct !DIGlobalVariable(scope: null, file: !2, line: 471, type: !121, isLocal: true, isDefinition: true)
!146 = !DIGlobalVariableExpression(var: !147, expr: !DIExpression())
!147 = distinct !DIGlobalVariable(scope: null, file: !2, line: 476, type: !148, isLocal: true, isDefinition: true)
!148 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 128, elements: !149)
!149 = !{!150}
!150 = !DISubrange(count: 16)
!151 = !DIGlobalVariableExpression(var: !152, expr: !DIExpression())
!152 = distinct !DIGlobalVariable(scope: null, file: !2, line: 478, type: !141, isLocal: true, isDefinition: true)
!153 = !DIGlobalVariableExpression(var: !154, expr: !DIExpression())
!154 = distinct !DIGlobalVariable(scope: null, file: !2, line: 483, type: !126, isLocal: true, isDefinition: true)
!155 = !DIGlobalVariableExpression(var: !156, expr: !DIExpression())
!156 = distinct !DIGlobalVariable(scope: null, file: !2, line: 487, type: !157, isLocal: true, isDefinition: true)
!157 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 224, elements: !158)
!158 = !{!159}
!159 = !DISubrange(count: 28)
!160 = !DIGlobalVariableExpression(var: !161, expr: !DIExpression())
!161 = distinct !DIGlobalVariable(scope: null, file: !2, line: 491, type: !162, isLocal: true, isDefinition: true)
!162 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 208, elements: !163)
!163 = !{!164}
!164 = !DISubrange(count: 26)
!165 = !DIGlobalVariableExpression(var: !166, expr: !DIExpression())
!166 = distinct !DIGlobalVariable(scope: null, file: !2, line: 498, type: !136, isLocal: true, isDefinition: true)
!167 = !DIGlobalVariableExpression(var: !168, expr: !DIExpression())
!168 = distinct !DIGlobalVariable(scope: null, file: !2, line: 500, type: !111, isLocal: true, isDefinition: true)
!169 = !DIGlobalVariableExpression(var: !170, expr: !DIExpression())
!170 = distinct !DIGlobalVariable(scope: null, file: !2, line: 847, type: !171, isLocal: true, isDefinition: true)
!171 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 56, elements: !172)
!172 = !{!173}
!173 = !DISubrange(count: 7)
!174 = !DIGlobalVariableExpression(var: !175, expr: !DIExpression())
!175 = distinct !DIGlobalVariable(scope: null, file: !2, line: 847, type: !136, isLocal: true, isDefinition: true)
!176 = !DIGlobalVariableExpression(var: !177, expr: !DIExpression())
!177 = distinct !DIGlobalVariable(scope: null, file: !2, line: 847, type: !178, isLocal: true, isDefinition: true)
!178 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 104, elements: !179)
!179 = !{!180}
!180 = !DISubrange(count: 13)
!181 = !DIGlobalVariableExpression(var: !182, expr: !DIExpression())
!182 = distinct !DIGlobalVariable(scope: null, file: !2, line: 850, type: !131, isLocal: true, isDefinition: true)
!183 = !DIGlobalVariableExpression(var: !184, expr: !DIExpression())
!184 = distinct !DIGlobalVariable(scope: null, file: !2, line: 850, type: !94, isLocal: true, isDefinition: true)
!185 = !DIGlobalVariableExpression(var: !186, expr: !DIExpression())
!186 = distinct !DIGlobalVariable(scope: null, file: !2, line: 853, type: !3, isLocal: true, isDefinition: true)
!187 = !DIGlobalVariableExpression(var: !188, expr: !DIExpression())
!188 = distinct !DIGlobalVariable(scope: null, file: !2, line: 853, type: !94, isLocal: true, isDefinition: true)
!189 = !DIGlobalVariableExpression(var: !190, expr: !DIExpression())
!190 = distinct !DIGlobalVariable(scope: null, file: !2, line: 856, type: !171, isLocal: true, isDefinition: true)
!191 = !DIGlobalVariableExpression(var: !192, expr: !DIExpression())
!192 = distinct !DIGlobalVariable(scope: null, file: !2, line: 856, type: !178, isLocal: true, isDefinition: true)
!193 = !DIGlobalVariableExpression(var: !194, expr: !DIExpression())
!194 = distinct !DIGlobalVariable(scope: null, file: !2, line: 859, type: !178, isLocal: true, isDefinition: true)
!195 = !DIGlobalVariableExpression(var: !196, expr: !DIExpression())
!196 = distinct !DIGlobalVariable(scope: null, file: !2, line: 859, type: !197, isLocal: true, isDefinition: true)
!197 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 152, elements: !198)
!198 = !{!199}
!199 = !DISubrange(count: 19)
!200 = !DIGlobalVariableExpression(var: !201, expr: !DIExpression())
!201 = distinct !DIGlobalVariable(scope: null, file: !2, line: 862, type: !202, isLocal: true, isDefinition: true)
!202 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 80, elements: !203)
!203 = !{!204}
!204 = !DISubrange(count: 10)
!205 = !DIGlobalVariableExpression(var: !206, expr: !DIExpression())
!206 = distinct !DIGlobalVariable(scope: null, file: !2, line: 862, type: !148, isLocal: true, isDefinition: true)
!207 = !DIGlobalVariableExpression(var: !208, expr: !DIExpression())
!208 = distinct !DIGlobalVariable(scope: null, file: !2, line: 865, type: !178, isLocal: true, isDefinition: true)
!209 = !DIGlobalVariableExpression(var: !210, expr: !DIExpression())
!210 = distinct !DIGlobalVariable(scope: null, file: !2, line: 865, type: !197, isLocal: true, isDefinition: true)
!211 = !DIGlobalVariableExpression(var: !212, expr: !DIExpression())
!212 = distinct !DIGlobalVariable(scope: null, file: !2, line: 867, type: !126, isLocal: true, isDefinition: true)
!213 = !DIGlobalVariableExpression(var: !214, expr: !DIExpression())
!214 = distinct !DIGlobalVariable(scope: null, file: !2, line: 868, type: !111, isLocal: true, isDefinition: true)
!215 = !DIGlobalVariableExpression(var: !216, expr: !DIExpression())
!216 = distinct !DIGlobalVariable(scope: null, file: !2, line: 875, type: !148, isLocal: true, isDefinition: true)
!217 = !DIGlobalVariableExpression(var: !218, expr: !DIExpression())
!218 = distinct !DIGlobalVariable(scope: null, file: !2, line: 876, type: !94, isLocal: true, isDefinition: true)
!219 = !DIGlobalVariableExpression(var: !220, expr: !DIExpression())
!220 = distinct !DIGlobalVariable(scope: null, file: !2, line: 876, type: !131, isLocal: true, isDefinition: true)
!221 = !DIGlobalVariableExpression(var: !222, expr: !DIExpression())
!222 = distinct !DIGlobalVariable(scope: null, file: !2, line: 876, type: !136, isLocal: true, isDefinition: true)
!223 = !DIGlobalVariableExpression(var: !224, expr: !DIExpression())
!224 = distinct !DIGlobalVariable(scope: null, file: !2, line: 877, type: !178, isLocal: true, isDefinition: true)
!225 = !DIGlobalVariableExpression(var: !226, expr: !DIExpression())
!226 = distinct !DIGlobalVariable(scope: null, file: !2, line: 878, type: !148, isLocal: true, isDefinition: true)
!227 = !DIGlobalVariableExpression(var: !228, expr: !DIExpression())
!228 = distinct !DIGlobalVariable(scope: null, file: !2, line: 880, type: !229, isLocal: true, isDefinition: true)
!229 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 176, elements: !230)
!230 = !{!231}
!231 = !DISubrange(count: 22)
!232 = !DIGlobalVariableExpression(var: !233, expr: !DIExpression())
!233 = distinct !DIGlobalVariable(scope: null, file: !2, line: 882, type: !234, isLocal: true, isDefinition: true)
!234 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 168, elements: !235)
!235 = !{!236}
!236 = !DISubrange(count: 21)
!237 = !DIGlobalVariableExpression(var: !238, expr: !DIExpression())
!238 = distinct !DIGlobalVariable(scope: null, file: !2, line: 885, type: !197, isLocal: true, isDefinition: true)
!239 = !DIGlobalVariableExpression(var: !240, expr: !DIExpression())
!240 = distinct !DIGlobalVariable(scope: null, file: !2, line: 887, type: !141, isLocal: true, isDefinition: true)
!241 = !DIGlobalVariableExpression(var: !242, expr: !DIExpression())
!242 = distinct !DIGlobalVariable(scope: null, file: !2, line: 890, type: !229, isLocal: true, isDefinition: true)
!243 = !DIGlobalVariableExpression(var: !244, expr: !DIExpression())
!244 = distinct !DIGlobalVariable(scope: null, file: !2, line: 892, type: !245, isLocal: true, isDefinition: true)
!245 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 192, elements: !246)
!246 = !{!247}
!247 = !DISubrange(count: 24)
!248 = !DIGlobalVariableExpression(var: !249, expr: !DIExpression())
!249 = distinct !DIGlobalVariable(scope: null, file: !2, line: 897, type: !47, isLocal: true, isDefinition: true)
!250 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !2, line: 207, size: 213440, elements: !251)
!251 = !{!252, !253, !312, !313, !317, !318, !327, !328, !329, !330, !331}
!252 = !DIDerivedType(tag: DW_TAG_member, name: "flags_count", scope: !250, file: !2, line: 208, baseType: !38, size: 32)
!253 = !DIDerivedType(tag: DW_TAG_member, name: "flags", scope: !250, file: !2, line: 209, baseType: !254, size: 81920, offset: 64)
!254 = !DICompositeType(tag: DW_TAG_array_type, baseType: !255, size: 81920, elements: !310)
!255 = !DIDerivedType(tag: DW_TAG_typedef, name: "_kgflags_flag_t", file: !2, line: 172, baseType: !256)
!256 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_kgflags_flag", file: !2, line: 150, size: 320, elements: !257)
!257 = !{!258, !261, !262, !271, !305, !306, !307, !308}
!258 = !DIDerivedType(tag: DW_TAG_member, name: "name", scope: !256, file: !2, line: 151, baseType: !259, size: 64)
!259 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !260, size: 64)
!260 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !4)
!261 = !DIDerivedType(tag: DW_TAG_member, name: "description", scope: !256, file: !2, line: 152, baseType: !259, size: 64, offset: 64)
!262 = !DIDerivedType(tag: DW_TAG_member, name: "default_value", scope: !256, file: !2, line: 158, baseType: !263, size: 64, offset: 128)
!263 = distinct !DICompositeType(tag: DW_TAG_union_type, scope: !256, file: !2, line: 153, size: 64, elements: !264)
!264 = !{!265, !266, !268, !269}
!265 = !DIDerivedType(tag: DW_TAG_member, name: "string_value", scope: !263, file: !2, line: 154, baseType: !259, size: 64)
!266 = !DIDerivedType(tag: DW_TAG_member, name: "bool_value", scope: !263, file: !2, line: 155, baseType: !267, size: 8)
!267 = !DIBasicType(name: "_Bool", size: 8, encoding: DW_ATE_boolean)
!268 = !DIDerivedType(tag: DW_TAG_member, name: "int_value", scope: !263, file: !2, line: 156, baseType: !38, size: 32)
!269 = !DIDerivedType(tag: DW_TAG_member, name: "double_value", scope: !263, file: !2, line: 157, baseType: !270, size: 64)
!270 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!271 = !DIDerivedType(tag: DW_TAG_member, name: "result", scope: !256, file: !2, line: 167, baseType: !272, size: 64, offset: 192)
!272 = distinct !DICompositeType(tag: DW_TAG_union_type, scope: !256, file: !2, line: 159, size: 64, elements: !273)
!273 = !{!274, !276, !278, !280, !282, !291, !298}
!274 = !DIDerivedType(tag: DW_TAG_member, name: "string_value", scope: !272, file: !2, line: 160, baseType: !275, size: 64)
!275 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !259, size: 64)
!276 = !DIDerivedType(tag: DW_TAG_member, name: "bool_value", scope: !272, file: !2, line: 161, baseType: !277, size: 64)
!277 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !267, size: 64)
!278 = !DIDerivedType(tag: DW_TAG_member, name: "int_value", scope: !272, file: !2, line: 162, baseType: !279, size: 64)
!279 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !38, size: 64)
!280 = !DIDerivedType(tag: DW_TAG_member, name: "double_value", scope: !272, file: !2, line: 163, baseType: !281, size: 64)
!281 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !270, size: 64)
!282 = !DIDerivedType(tag: DW_TAG_member, name: "string_array", scope: !272, file: !2, line: 164, baseType: !283, size: 64)
!283 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !284, size: 64)
!284 = !DIDerivedType(tag: DW_TAG_typedef, name: "kgflags_string_array_t", file: !2, line: 56, baseType: !285)
!285 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "kgflags_string_array", file: !2, line: 53, size: 128, elements: !286)
!286 = !{!287, !290}
!287 = !DIDerivedType(tag: DW_TAG_member, name: "_items", scope: !285, file: !2, line: 54, baseType: !288, size: 64)
!288 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !289, size: 64)
!289 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !4, size: 64)
!290 = !DIDerivedType(tag: DW_TAG_member, name: "_count", scope: !285, file: !2, line: 55, baseType: !38, size: 32, offset: 64)
!291 = !DIDerivedType(tag: DW_TAG_member, name: "int_array", scope: !272, file: !2, line: 165, baseType: !292, size: 64)
!292 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !293, size: 64)
!293 = !DIDerivedType(tag: DW_TAG_typedef, name: "kgflags_int_array_t", file: !2, line: 61, baseType: !294)
!294 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "kgflags_int_array", file: !2, line: 58, size: 128, elements: !295)
!295 = !{!296, !297}
!296 = !DIDerivedType(tag: DW_TAG_member, name: "_items", scope: !294, file: !2, line: 59, baseType: !288, size: 64)
!297 = !DIDerivedType(tag: DW_TAG_member, name: "_count", scope: !294, file: !2, line: 60, baseType: !38, size: 32, offset: 64)
!298 = !DIDerivedType(tag: DW_TAG_member, name: "double_array", scope: !272, file: !2, line: 166, baseType: !299, size: 64)
!299 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !300, size: 64)
!300 = !DIDerivedType(tag: DW_TAG_typedef, name: "kgflags_double_array_t", file: !2, line: 66, baseType: !301)
!301 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "kgflags_double_array", file: !2, line: 63, size: 128, elements: !302)
!302 = !{!303, !304}
!303 = !DIDerivedType(tag: DW_TAG_member, name: "_items", scope: !301, file: !2, line: 64, baseType: !288, size: 64)
!304 = !DIDerivedType(tag: DW_TAG_member, name: "_count", scope: !301, file: !2, line: 65, baseType: !38, size: 32, offset: 64)
!305 = !DIDerivedType(tag: DW_TAG_member, name: "assigned", scope: !256, file: !2, line: 168, baseType: !267, size: 8, offset: 256)
!306 = !DIDerivedType(tag: DW_TAG_member, name: "error", scope: !256, file: !2, line: 169, baseType: !267, size: 8, offset: 264)
!307 = !DIDerivedType(tag: DW_TAG_member, name: "required", scope: !256, file: !2, line: 170, baseType: !267, size: 8, offset: 272)
!308 = !DIDerivedType(tag: DW_TAG_member, name: "kind", scope: !256, file: !2, line: 171, baseType: !309, size: 32, offset: 288)
!309 = !DIDerivedType(tag: DW_TAG_typedef, name: "_kgflags_flag_kind_t", file: !2, line: 148, baseType: !12)
!310 = !{!311}
!311 = !DISubrange(count: 256)
!312 = !DIDerivedType(tag: DW_TAG_member, name: "non_flag_count", scope: !250, file: !2, line: 211, baseType: !38, size: 32, offset: 81984)
!313 = !DIDerivedType(tag: DW_TAG_member, name: "non_flag_args", scope: !250, file: !2, line: 212, baseType: !314, size: 32768, offset: 82048)
!314 = !DICompositeType(tag: DW_TAG_array_type, baseType: !259, size: 32768, elements: !315)
!315 = !{!316}
!316 = !DISubrange(count: 512)
!317 = !DIDerivedType(tag: DW_TAG_member, name: "errors_count", scope: !250, file: !2, line: 214, baseType: !38, size: 32, offset: 114816)
!318 = !DIDerivedType(tag: DW_TAG_member, name: "errors", scope: !250, file: !2, line: 215, baseType: !319, size: 98304, offset: 114880)
!319 = !DICompositeType(tag: DW_TAG_array_type, baseType: !320, size: 98304, elements: !315)
!320 = !DIDerivedType(tag: DW_TAG_typedef, name: "_kgflags_error_t", file: !2, line: 192, baseType: !321)
!321 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "_kgflags_error", file: !2, line: 188, size: 192, elements: !322)
!322 = !{!323, !324, !325}
!323 = !DIDerivedType(tag: DW_TAG_member, name: "flag_name", scope: !321, file: !2, line: 189, baseType: !259, size: 64)
!324 = !DIDerivedType(tag: DW_TAG_member, name: "arg", scope: !321, file: !2, line: 190, baseType: !259, size: 64, offset: 64)
!325 = !DIDerivedType(tag: DW_TAG_member, name: "kind", scope: !321, file: !2, line: 191, baseType: !326, size: 32, offset: 128)
!326 = !DIDerivedType(tag: DW_TAG_typedef, name: "_kgflags_error_kind_t", file: !2, line: 186, baseType: !23)
!327 = !DIDerivedType(tag: DW_TAG_member, name: "flag_prefix", scope: !250, file: !2, line: 217, baseType: !259, size: 64, offset: 213184)
!328 = !DIDerivedType(tag: DW_TAG_member, name: "arg_cursor", scope: !250, file: !2, line: 219, baseType: !38, size: 32, offset: 213248)
!329 = !DIDerivedType(tag: DW_TAG_member, name: "argc", scope: !250, file: !2, line: 220, baseType: !38, size: 32, offset: 213280)
!330 = !DIDerivedType(tag: DW_TAG_member, name: "argv", scope: !250, file: !2, line: 221, baseType: !288, size: 64, offset: 213312)
!331 = !DIDerivedType(tag: DW_TAG_member, name: "custom_description", scope: !250, file: !2, line: 223, baseType: !259, size: 64, offset: 213376)
!332 = !{i32 7, !"Dwarf Version", i32 5}
!333 = !{i32 2, !"Debug Info Version", i32 3}
!334 = !{i32 1, !"wchar_size", i32 4}
!335 = !{i32 8, !"PIC Level", i32 2}
!336 = !{i32 7, !"PIE Level", i32 2}
!337 = !{i32 7, !"uwtable", i32 2}
!338 = !{i32 7, !"frame-pointer", i32 2}
!339 = !{!"clang version 20.1.8"}
!340 = distinct !DISubprogram(name: "kgflags_string", scope: !2, file: !2, line: 226, type: !341, scopeLine: 226, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!341 = !DISubroutineType(types: !342)
!342 = !{null, !259, !259, !259, !267, !275}
!343 = !{}
!344 = !DILocalVariable(name: "name", arg: 1, scope: !340, file: !2, line: 226, type: !259)
!345 = !DILocation(line: 226, column: 33, scope: !340)
!346 = !DILocalVariable(name: "default_value", arg: 2, scope: !340, file: !2, line: 226, type: !259)
!347 = !DILocation(line: 226, column: 51, scope: !340)
!348 = !DILocalVariable(name: "description", arg: 3, scope: !340, file: !2, line: 226, type: !259)
!349 = !DILocation(line: 226, column: 78, scope: !340)
!350 = !DILocalVariable(name: "required", arg: 4, scope: !340, file: !2, line: 226, type: !267)
!351 = !DILocation(line: 226, column: 96, scope: !340)
!352 = !DILocalVariable(name: "out_res", arg: 5, scope: !340, file: !2, line: 226, type: !275)
!353 = !DILocation(line: 226, column: 119, scope: !340)
!354 = !DILocation(line: 227, column: 6, scope: !340)
!355 = !DILocation(line: 227, column: 14, scope: !340)
!356 = !DILocalVariable(name: "flag", scope: !340, file: !2, line: 229, type: !255)
!357 = !DILocation(line: 229, column: 21, scope: !340)
!358 = !DILocation(line: 230, column: 5, scope: !340)
!359 = !DILocation(line: 231, column: 10, scope: !340)
!360 = !DILocation(line: 231, column: 15, scope: !340)
!361 = !DILocation(line: 232, column: 17, scope: !340)
!362 = !DILocation(line: 232, column: 10, scope: !340)
!363 = !DILocation(line: 232, column: 15, scope: !340)
!364 = !DILocation(line: 233, column: 39, scope: !340)
!365 = !DILocation(line: 233, column: 10, scope: !340)
!366 = !DILocation(line: 233, column: 37, scope: !340)
!367 = !DILocation(line: 234, column: 24, scope: !340)
!368 = !DILocation(line: 234, column: 10, scope: !340)
!369 = !DILocation(line: 234, column: 22, scope: !340)
!370 = !DILocation(line: 235, column: 21, scope: !340)
!371 = !DILocation(line: 235, column: 10, scope: !340)
!372 = !DILocation(line: 235, column: 19, scope: !340)
!373 = !DILocation(line: 236, column: 32, scope: !340)
!374 = !DILocation(line: 236, column: 10, scope: !340)
!375 = !DILocation(line: 236, column: 30, scope: !340)
!376 = !DILocation(line: 237, column: 10, scope: !340)
!377 = !DILocation(line: 237, column: 19, scope: !340)
!378 = !DILocation(line: 238, column: 5, scope: !340)
!379 = !DILocation(line: 239, column: 1, scope: !340)
!380 = distinct !DISubprogram(name: "_kgflags_add_flag", scope: !2, file: !2, line: 583, type: !381, scopeLine: 583, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !9, retainedNodes: !343)
!381 = !DISubroutineType(types: !382)
!382 = !{null, !255}
!383 = !DILocalVariable(name: "flag", arg: 1, scope: !380, file: !2, line: 583, type: !255)
!384 = !DILocation(line: 583, column: 47, scope: !380)
!385 = !DILocation(line: 584, column: 32, scope: !386)
!386 = distinct !DILexicalBlock(scope: !380, file: !2, line: 584, column: 9)
!387 = !DILocation(line: 584, column: 9, scope: !386)
!388 = !DILocation(line: 584, column: 44, scope: !386)
!389 = !DILocation(line: 585, column: 68, scope: !390)
!390 = distinct !DILexicalBlock(scope: !386, file: !2, line: 584, column: 53)
!391 = !DILocation(line: 585, column: 9, scope: !390)
!392 = !DILocation(line: 586, column: 9, scope: !390)
!393 = !DILocation(line: 588, column: 20, scope: !394)
!394 = distinct !DILexicalBlock(scope: !380, file: !2, line: 588, column: 9)
!395 = !DILocation(line: 588, column: 32, scope: !394)
!396 = !DILocation(line: 589, column: 9, scope: !397)
!397 = distinct !DILexicalBlock(scope: !394, file: !2, line: 588, column: 54)
!398 = !DILocation(line: 590, column: 9, scope: !397)
!399 = !DILocation(line: 592, column: 33, scope: !380)
!400 = !DILocation(line: 592, column: 5, scope: !380)
!401 = !DILocation(line: 592, column: 48, scope: !380)
!402 = !DILocation(line: 593, column: 27, scope: !380)
!403 = !DILocation(line: 594, column: 1, scope: !380)
!404 = distinct !DISubprogram(name: "kgflags_bool", scope: !2, file: !2, line: 241, type: !405, scopeLine: 241, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!405 = !DISubroutineType(types: !406)
!406 = !{null, !259, !267, !259, !267, !277}
!407 = !DILocalVariable(name: "name", arg: 1, scope: !404, file: !2, line: 241, type: !259)
!408 = !DILocation(line: 241, column: 31, scope: !404)
!409 = !DILocalVariable(name: "default_value", arg: 2, scope: !404, file: !2, line: 241, type: !267)
!410 = !DILocation(line: 241, column: 42, scope: !404)
!411 = !DILocalVariable(name: "description", arg: 3, scope: !404, file: !2, line: 241, type: !259)
!412 = !DILocation(line: 241, column: 69, scope: !404)
!413 = !DILocalVariable(name: "required", arg: 4, scope: !404, file: !2, line: 241, type: !267)
!414 = !DILocation(line: 241, column: 87, scope: !404)
!415 = !DILocalVariable(name: "out_res", arg: 5, scope: !404, file: !2, line: 241, type: !277)
!416 = !DILocation(line: 241, column: 103, scope: !404)
!417 = !DILocation(line: 242, column: 6, scope: !404)
!418 = !DILocation(line: 242, column: 14, scope: !404)
!419 = !DILocation(line: 244, column: 16, scope: !420)
!420 = distinct !DILexicalBlock(scope: !404, file: !2, line: 244, column: 9)
!421 = !DILocation(line: 244, column: 9, scope: !420)
!422 = !DILocation(line: 244, column: 32, scope: !420)
!423 = !DILocation(line: 244, column: 29, scope: !420)
!424 = !DILocation(line: 245, column: 58, scope: !425)
!425 = distinct !DILexicalBlock(scope: !420, file: !2, line: 244, column: 38)
!426 = !DILocation(line: 245, column: 9, scope: !425)
!427 = !DILocation(line: 246, column: 9, scope: !425)
!428 = !DILocalVariable(name: "flag", scope: !404, file: !2, line: 249, type: !255)
!429 = !DILocation(line: 249, column: 21, scope: !404)
!430 = !DILocation(line: 250, column: 5, scope: !404)
!431 = !DILocation(line: 251, column: 10, scope: !404)
!432 = !DILocation(line: 251, column: 15, scope: !404)
!433 = !DILocation(line: 252, column: 17, scope: !404)
!434 = !DILocation(line: 252, column: 10, scope: !404)
!435 = !DILocation(line: 252, column: 15, scope: !404)
!436 = !DILocation(line: 253, column: 37, scope: !404)
!437 = !DILocation(line: 253, column: 10, scope: !404)
!438 = !DILocation(line: 253, column: 35, scope: !404)
!439 = !DILocation(line: 254, column: 24, scope: !404)
!440 = !DILocation(line: 254, column: 10, scope: !404)
!441 = !DILocation(line: 254, column: 22, scope: !404)
!442 = !DILocation(line: 255, column: 21, scope: !404)
!443 = !DILocation(line: 255, column: 10, scope: !404)
!444 = !DILocation(line: 255, column: 19, scope: !404)
!445 = !DILocation(line: 256, column: 30, scope: !404)
!446 = !DILocation(line: 256, column: 10, scope: !404)
!447 = !DILocation(line: 256, column: 28, scope: !404)
!448 = !DILocation(line: 257, column: 10, scope: !404)
!449 = !DILocation(line: 257, column: 19, scope: !404)
!450 = !DILocation(line: 258, column: 5, scope: !404)
!451 = !DILocation(line: 259, column: 1, scope: !404)
!452 = distinct !DISubprogram(name: "_kgflags_add_error", scope: !2, file: !2, line: 643, type: !453, scopeLine: 643, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !9, retainedNodes: !343)
!453 = !DISubroutineType(types: !454)
!454 = !{null, !326, !259, !259}
!455 = !DILocalVariable(name: "kind", arg: 1, scope: !452, file: !2, line: 643, type: !326)
!456 = !DILocation(line: 643, column: 54, scope: !452)
!457 = !DILocalVariable(name: "flag_name", arg: 2, scope: !452, file: !2, line: 643, type: !259)
!458 = !DILocation(line: 643, column: 72, scope: !452)
!459 = !DILocalVariable(name: "arg", arg: 3, scope: !452, file: !2, line: 643, type: !259)
!460 = !DILocation(line: 643, column: 95, scope: !452)
!461 = !DILocalVariable(name: "err", scope: !452, file: !2, line: 644, type: !320)
!462 = !DILocation(line: 644, column: 22, scope: !452)
!463 = !DILocation(line: 645, column: 16, scope: !452)
!464 = !DILocation(line: 645, column: 9, scope: !452)
!465 = !DILocation(line: 645, column: 14, scope: !452)
!466 = !DILocation(line: 646, column: 21, scope: !452)
!467 = !DILocation(line: 646, column: 9, scope: !452)
!468 = !DILocation(line: 646, column: 19, scope: !452)
!469 = !DILocation(line: 647, column: 15, scope: !452)
!470 = !DILocation(line: 647, column: 9, scope: !452)
!471 = !DILocation(line: 647, column: 13, scope: !452)
!472 = !DILocation(line: 648, column: 20, scope: !473)
!473 = distinct !DILexicalBlock(scope: !452, file: !2, line: 648, column: 9)
!474 = !DILocation(line: 648, column: 33, scope: !473)
!475 = !DILocation(line: 649, column: 9, scope: !476)
!476 = distinct !DILexicalBlock(scope: !473, file: !2, line: 648, column: 56)
!477 = !DILocation(line: 651, column: 34, scope: !452)
!478 = !DILocation(line: 651, column: 5, scope: !452)
!479 = !DILocation(line: 651, column: 50, scope: !452)
!480 = !DILocation(line: 652, column: 28, scope: !452)
!481 = !DILocation(line: 653, column: 1, scope: !452)
!482 = distinct !DISubprogram(name: "kgflags_int", scope: !2, file: !2, line: 261, type: !483, scopeLine: 261, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!483 = !DISubroutineType(types: !484)
!484 = !{null, !259, !38, !259, !267, !279}
!485 = !DILocalVariable(name: "name", arg: 1, scope: !482, file: !2, line: 261, type: !259)
!486 = !DILocation(line: 261, column: 30, scope: !482)
!487 = !DILocalVariable(name: "default_value", arg: 2, scope: !482, file: !2, line: 261, type: !38)
!488 = !DILocation(line: 261, column: 40, scope: !482)
!489 = !DILocalVariable(name: "description", arg: 3, scope: !482, file: !2, line: 261, type: !259)
!490 = !DILocation(line: 261, column: 67, scope: !482)
!491 = !DILocalVariable(name: "required", arg: 4, scope: !482, file: !2, line: 261, type: !267)
!492 = !DILocation(line: 261, column: 85, scope: !482)
!493 = !DILocalVariable(name: "out_res", arg: 5, scope: !482, file: !2, line: 261, type: !279)
!494 = !DILocation(line: 261, column: 100, scope: !482)
!495 = !DILocation(line: 262, column: 6, scope: !482)
!496 = !DILocation(line: 262, column: 14, scope: !482)
!497 = !DILocalVariable(name: "flag", scope: !482, file: !2, line: 264, type: !255)
!498 = !DILocation(line: 264, column: 21, scope: !482)
!499 = !DILocation(line: 265, column: 5, scope: !482)
!500 = !DILocation(line: 266, column: 10, scope: !482)
!501 = !DILocation(line: 266, column: 15, scope: !482)
!502 = !DILocation(line: 267, column: 17, scope: !482)
!503 = !DILocation(line: 267, column: 10, scope: !482)
!504 = !DILocation(line: 267, column: 15, scope: !482)
!505 = !DILocation(line: 268, column: 36, scope: !482)
!506 = !DILocation(line: 268, column: 10, scope: !482)
!507 = !DILocation(line: 268, column: 34, scope: !482)
!508 = !DILocation(line: 269, column: 24, scope: !482)
!509 = !DILocation(line: 269, column: 10, scope: !482)
!510 = !DILocation(line: 269, column: 22, scope: !482)
!511 = !DILocation(line: 270, column: 21, scope: !482)
!512 = !DILocation(line: 270, column: 10, scope: !482)
!513 = !DILocation(line: 270, column: 19, scope: !482)
!514 = !DILocation(line: 271, column: 29, scope: !482)
!515 = !DILocation(line: 271, column: 10, scope: !482)
!516 = !DILocation(line: 271, column: 27, scope: !482)
!517 = !DILocation(line: 272, column: 10, scope: !482)
!518 = !DILocation(line: 272, column: 19, scope: !482)
!519 = !DILocation(line: 273, column: 5, scope: !482)
!520 = !DILocation(line: 274, column: 1, scope: !482)
!521 = distinct !DISubprogram(name: "kgflags_double", scope: !2, file: !2, line: 276, type: !522, scopeLine: 276, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!522 = !DISubroutineType(types: !523)
!523 = !{null, !259, !270, !259, !267, !281}
!524 = !DILocalVariable(name: "name", arg: 1, scope: !521, file: !2, line: 276, type: !259)
!525 = !DILocation(line: 276, column: 33, scope: !521)
!526 = !DILocalVariable(name: "default_value", arg: 2, scope: !521, file: !2, line: 276, type: !270)
!527 = !DILocation(line: 276, column: 46, scope: !521)
!528 = !DILocalVariable(name: "description", arg: 3, scope: !521, file: !2, line: 276, type: !259)
!529 = !DILocation(line: 276, column: 73, scope: !521)
!530 = !DILocalVariable(name: "required", arg: 4, scope: !521, file: !2, line: 276, type: !267)
!531 = !DILocation(line: 276, column: 91, scope: !521)
!532 = !DILocalVariable(name: "out_res", arg: 5, scope: !521, file: !2, line: 276, type: !281)
!533 = !DILocation(line: 276, column: 109, scope: !521)
!534 = !DILocation(line: 277, column: 6, scope: !521)
!535 = !DILocation(line: 277, column: 14, scope: !521)
!536 = !DILocalVariable(name: "flag", scope: !521, file: !2, line: 279, type: !255)
!537 = !DILocation(line: 279, column: 21, scope: !521)
!538 = !DILocation(line: 280, column: 5, scope: !521)
!539 = !DILocation(line: 281, column: 10, scope: !521)
!540 = !DILocation(line: 281, column: 15, scope: !521)
!541 = !DILocation(line: 282, column: 17, scope: !521)
!542 = !DILocation(line: 282, column: 10, scope: !521)
!543 = !DILocation(line: 282, column: 15, scope: !521)
!544 = !DILocation(line: 283, column: 39, scope: !521)
!545 = !DILocation(line: 283, column: 10, scope: !521)
!546 = !DILocation(line: 283, column: 37, scope: !521)
!547 = !DILocation(line: 284, column: 24, scope: !521)
!548 = !DILocation(line: 284, column: 10, scope: !521)
!549 = !DILocation(line: 284, column: 22, scope: !521)
!550 = !DILocation(line: 285, column: 21, scope: !521)
!551 = !DILocation(line: 285, column: 10, scope: !521)
!552 = !DILocation(line: 285, column: 19, scope: !521)
!553 = !DILocation(line: 286, column: 32, scope: !521)
!554 = !DILocation(line: 286, column: 10, scope: !521)
!555 = !DILocation(line: 286, column: 30, scope: !521)
!556 = !DILocation(line: 287, column: 10, scope: !521)
!557 = !DILocation(line: 287, column: 19, scope: !521)
!558 = !DILocation(line: 288, column: 5, scope: !521)
!559 = !DILocation(line: 289, column: 1, scope: !521)
!560 = distinct !DISubprogram(name: "kgflags_string_array", scope: !2, file: !2, line: 291, type: !561, scopeLine: 291, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!561 = !DISubroutineType(types: !562)
!562 = !{null, !259, !259, !267, !283}
!563 = !DILocalVariable(name: "name", arg: 1, scope: !560, file: !2, line: 291, type: !259)
!564 = !DILocation(line: 291, column: 39, scope: !560)
!565 = !DILocalVariable(name: "description", arg: 2, scope: !560, file: !2, line: 291, type: !259)
!566 = !DILocation(line: 291, column: 57, scope: !560)
!567 = !DILocalVariable(name: "required", arg: 3, scope: !560, file: !2, line: 291, type: !267)
!568 = !DILocation(line: 291, column: 75, scope: !560)
!569 = !DILocalVariable(name: "out_arr", arg: 4, scope: !560, file: !2, line: 291, type: !283)
!570 = !DILocation(line: 291, column: 109, scope: !560)
!571 = !DILocation(line: 292, column: 5, scope: !560)
!572 = !DILocation(line: 292, column: 14, scope: !560)
!573 = !DILocation(line: 292, column: 21, scope: !560)
!574 = !DILocation(line: 293, column: 5, scope: !560)
!575 = !DILocation(line: 293, column: 14, scope: !560)
!576 = !DILocation(line: 293, column: 21, scope: !560)
!577 = !DILocalVariable(name: "flag", scope: !560, file: !2, line: 295, type: !255)
!578 = !DILocation(line: 295, column: 21, scope: !560)
!579 = !DILocation(line: 296, column: 5, scope: !560)
!580 = !DILocation(line: 297, column: 10, scope: !560)
!581 = !DILocation(line: 297, column: 15, scope: !560)
!582 = !DILocation(line: 298, column: 17, scope: !560)
!583 = !DILocation(line: 298, column: 10, scope: !560)
!584 = !DILocation(line: 298, column: 15, scope: !560)
!585 = !DILocation(line: 299, column: 24, scope: !560)
!586 = !DILocation(line: 299, column: 10, scope: !560)
!587 = !DILocation(line: 299, column: 22, scope: !560)
!588 = !DILocation(line: 300, column: 21, scope: !560)
!589 = !DILocation(line: 300, column: 10, scope: !560)
!590 = !DILocation(line: 300, column: 19, scope: !560)
!591 = !DILocation(line: 301, column: 32, scope: !560)
!592 = !DILocation(line: 301, column: 10, scope: !560)
!593 = !DILocation(line: 301, column: 30, scope: !560)
!594 = !DILocation(line: 302, column: 10, scope: !560)
!595 = !DILocation(line: 302, column: 19, scope: !560)
!596 = !DILocation(line: 303, column: 5, scope: !560)
!597 = !DILocation(line: 304, column: 1, scope: !560)
!598 = distinct !DISubprogram(name: "kgflags_int_array", scope: !2, file: !2, line: 306, type: !599, scopeLine: 306, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!599 = !DISubroutineType(types: !600)
!600 = !{null, !259, !259, !267, !292}
!601 = !DILocalVariable(name: "name", arg: 1, scope: !598, file: !2, line: 306, type: !259)
!602 = !DILocation(line: 306, column: 36, scope: !598)
!603 = !DILocalVariable(name: "description", arg: 2, scope: !598, file: !2, line: 306, type: !259)
!604 = !DILocation(line: 306, column: 54, scope: !598)
!605 = !DILocalVariable(name: "required", arg: 3, scope: !598, file: !2, line: 306, type: !267)
!606 = !DILocation(line: 306, column: 72, scope: !598)
!607 = !DILocalVariable(name: "out_arr", arg: 4, scope: !598, file: !2, line: 306, type: !292)
!608 = !DILocation(line: 306, column: 103, scope: !598)
!609 = !DILocation(line: 307, column: 5, scope: !598)
!610 = !DILocation(line: 307, column: 14, scope: !598)
!611 = !DILocation(line: 307, column: 21, scope: !598)
!612 = !DILocation(line: 308, column: 5, scope: !598)
!613 = !DILocation(line: 308, column: 14, scope: !598)
!614 = !DILocation(line: 308, column: 21, scope: !598)
!615 = !DILocalVariable(name: "flag", scope: !598, file: !2, line: 310, type: !255)
!616 = !DILocation(line: 310, column: 21, scope: !598)
!617 = !DILocation(line: 311, column: 5, scope: !598)
!618 = !DILocation(line: 312, column: 10, scope: !598)
!619 = !DILocation(line: 312, column: 15, scope: !598)
!620 = !DILocation(line: 313, column: 17, scope: !598)
!621 = !DILocation(line: 313, column: 10, scope: !598)
!622 = !DILocation(line: 313, column: 15, scope: !598)
!623 = !DILocation(line: 314, column: 24, scope: !598)
!624 = !DILocation(line: 314, column: 10, scope: !598)
!625 = !DILocation(line: 314, column: 22, scope: !598)
!626 = !DILocation(line: 315, column: 21, scope: !598)
!627 = !DILocation(line: 315, column: 10, scope: !598)
!628 = !DILocation(line: 315, column: 19, scope: !598)
!629 = !DILocation(line: 316, column: 29, scope: !598)
!630 = !DILocation(line: 316, column: 10, scope: !598)
!631 = !DILocation(line: 316, column: 27, scope: !598)
!632 = !DILocation(line: 317, column: 10, scope: !598)
!633 = !DILocation(line: 317, column: 19, scope: !598)
!634 = !DILocation(line: 318, column: 5, scope: !598)
!635 = !DILocation(line: 319, column: 1, scope: !598)
!636 = distinct !DISubprogram(name: "kgflags_double_array", scope: !2, file: !2, line: 321, type: !637, scopeLine: 321, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!637 = !DISubroutineType(types: !638)
!638 = !{null, !259, !259, !267, !299}
!639 = !DILocalVariable(name: "name", arg: 1, scope: !636, file: !2, line: 321, type: !259)
!640 = !DILocation(line: 321, column: 39, scope: !636)
!641 = !DILocalVariable(name: "description", arg: 2, scope: !636, file: !2, line: 321, type: !259)
!642 = !DILocation(line: 321, column: 57, scope: !636)
!643 = !DILocalVariable(name: "required", arg: 3, scope: !636, file: !2, line: 321, type: !267)
!644 = !DILocation(line: 321, column: 75, scope: !636)
!645 = !DILocalVariable(name: "out_arr", arg: 4, scope: !636, file: !2, line: 321, type: !299)
!646 = !DILocation(line: 321, column: 109, scope: !636)
!647 = !DILocation(line: 322, column: 5, scope: !636)
!648 = !DILocation(line: 322, column: 14, scope: !636)
!649 = !DILocation(line: 322, column: 21, scope: !636)
!650 = !DILocation(line: 323, column: 5, scope: !636)
!651 = !DILocation(line: 323, column: 14, scope: !636)
!652 = !DILocation(line: 323, column: 21, scope: !636)
!653 = !DILocalVariable(name: "flag", scope: !636, file: !2, line: 325, type: !255)
!654 = !DILocation(line: 325, column: 21, scope: !636)
!655 = !DILocation(line: 326, column: 5, scope: !636)
!656 = !DILocation(line: 327, column: 10, scope: !636)
!657 = !DILocation(line: 327, column: 15, scope: !636)
!658 = !DILocation(line: 328, column: 17, scope: !636)
!659 = !DILocation(line: 328, column: 10, scope: !636)
!660 = !DILocation(line: 328, column: 15, scope: !636)
!661 = !DILocation(line: 329, column: 24, scope: !636)
!662 = !DILocation(line: 329, column: 10, scope: !636)
!663 = !DILocation(line: 329, column: 22, scope: !636)
!664 = !DILocation(line: 330, column: 21, scope: !636)
!665 = !DILocation(line: 330, column: 10, scope: !636)
!666 = !DILocation(line: 330, column: 19, scope: !636)
!667 = !DILocation(line: 331, column: 32, scope: !636)
!668 = !DILocation(line: 331, column: 10, scope: !636)
!669 = !DILocation(line: 331, column: 30, scope: !636)
!670 = !DILocation(line: 332, column: 10, scope: !636)
!671 = !DILocation(line: 332, column: 19, scope: !636)
!672 = !DILocation(line: 333, column: 5, scope: !636)
!673 = !DILocation(line: 334, column: 1, scope: !636)
!674 = distinct !DISubprogram(name: "kgflags_set_prefix", scope: !2, file: !2, line: 336, type: !675, scopeLine: 336, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!675 = !DISubroutineType(types: !676)
!676 = !{null, !259}
!677 = !DILocalVariable(name: "prefix", arg: 1, scope: !674, file: !2, line: 336, type: !259)
!678 = !DILocation(line: 336, column: 37, scope: !674)
!679 = !DILocation(line: 337, column: 30, scope: !674)
!680 = !DILocation(line: 337, column: 28, scope: !674)
!681 = !DILocation(line: 338, column: 1, scope: !674)
!682 = distinct !DISubprogram(name: "kgflags_parse", scope: !2, file: !2, line: 340, type: !683, scopeLine: 340, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!683 = !DISubroutineType(types: !684)
!684 = !{!267, !38, !288}
!685 = !DILocalVariable(name: "argc", arg: 1, scope: !682, file: !2, line: 340, type: !38)
!686 = !DILocation(line: 340, column: 24, scope: !682)
!687 = !DILocalVariable(name: "argv", arg: 2, scope: !682, file: !2, line: 340, type: !288)
!688 = !DILocation(line: 340, column: 37, scope: !682)
!689 = !DILocation(line: 341, column: 23, scope: !682)
!690 = !DILocation(line: 341, column: 21, scope: !682)
!691 = !DILocation(line: 342, column: 23, scope: !682)
!692 = !DILocation(line: 342, column: 21, scope: !682)
!693 = !DILocation(line: 343, column: 27, scope: !682)
!694 = !DILocation(line: 345, column: 20, scope: !695)
!695 = distinct !DILexicalBlock(scope: !682, file: !2, line: 345, column: 9)
!696 = !DILocation(line: 345, column: 32, scope: !695)
!697 = !DILocation(line: 346, column: 32, scope: !698)
!698 = distinct !DILexicalBlock(scope: !695, file: !2, line: 345, column: 41)
!699 = !DILocation(line: 347, column: 5, scope: !698)
!700 = !DILocation(line: 349, column: 20, scope: !701)
!701 = distinct !DILexicalBlock(scope: !682, file: !2, line: 349, column: 9)
!702 = !DILocation(line: 349, column: 33, scope: !701)
!703 = !DILocation(line: 350, column: 9, scope: !704)
!704 = distinct !DILexicalBlock(scope: !701, file: !2, line: 349, column: 38)
!705 = !DILocalVariable(name: "arg", scope: !682, file: !2, line: 353, type: !259)
!706 = !DILocation(line: 353, column: 17, scope: !682)
!707 = !DILocation(line: 354, column: 5, scope: !682)
!708 = !DILocation(line: 354, column: 19, scope: !682)
!709 = !DILocation(line: 354, column: 17, scope: !682)
!710 = !DILocation(line: 354, column: 43, scope: !682)
!711 = !DILocalVariable(name: "flag", scope: !712, file: !2, line: 355, type: !713)
!712 = distinct !DILexicalBlock(scope: !682, file: !2, line: 354, column: 52)
!713 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !255, size: 64)
!714 = !DILocation(line: 355, column: 26, scope: !712)
!715 = !DILocalVariable(name: "is_flag", scope: !712, file: !2, line: 356, type: !267)
!716 = !DILocation(line: 356, column: 14, scope: !712)
!717 = !DILocation(line: 356, column: 41, scope: !712)
!718 = !DILocation(line: 356, column: 24, scope: !712)
!719 = !DILocalVariable(name: "prefix_no", scope: !712, file: !2, line: 357, type: !267)
!720 = !DILocation(line: 357, column: 14, scope: !712)
!721 = !DILocation(line: 358, column: 13, scope: !722)
!722 = distinct !DILexicalBlock(scope: !712, file: !2, line: 358, column: 13)
!723 = !DILocalVariable(name: "flag_name", scope: !724, file: !2, line: 359, type: !259)
!724 = distinct !DILexicalBlock(scope: !722, file: !2, line: 358, column: 22)
!725 = !DILocation(line: 359, column: 25, scope: !724)
!726 = !DILocation(line: 359, column: 60, scope: !724)
!727 = !DILocation(line: 359, column: 37, scope: !724)
!728 = !DILocation(line: 360, column: 38, scope: !724)
!729 = !DILocation(line: 360, column: 20, scope: !724)
!730 = !DILocation(line: 360, column: 18, scope: !724)
!731 = !DILocation(line: 361, column: 17, scope: !732)
!732 = distinct !DILexicalBlock(scope: !724, file: !2, line: 361, column: 17)
!733 = !DILocation(line: 361, column: 22, scope: !732)
!734 = !DILocation(line: 362, column: 69, scope: !735)
!735 = distinct !DILexicalBlock(scope: !732, file: !2, line: 361, column: 31)
!736 = !DILocation(line: 362, column: 17, scope: !735)
!737 = !DILocation(line: 363, column: 17, scope: !735)
!738 = distinct !{!738, !707, !739, !740}
!739 = !DILocation(line: 375, column: 5, scope: !682)
!740 = !{!"llvm.loop.mustprogress"}
!741 = !DILocation(line: 365, column: 9, scope: !724)
!742 = !DILocation(line: 366, column: 39, scope: !743)
!743 = distinct !DILexicalBlock(scope: !722, file: !2, line: 365, column: 16)
!744 = !DILocation(line: 366, column: 13, scope: !743)
!745 = !DILocation(line: 367, column: 13, scope: !743)
!746 = !DILocation(line: 370, column: 13, scope: !747)
!747 = distinct !DILexicalBlock(scope: !712, file: !2, line: 370, column: 13)
!748 = !DILocation(line: 370, column: 19, scope: !747)
!749 = !DILocation(line: 371, column: 72, scope: !750)
!750 = distinct !DILexicalBlock(scope: !747, file: !2, line: 370, column: 29)
!751 = !DILocation(line: 371, column: 78, scope: !750)
!752 = !DILocation(line: 371, column: 13, scope: !750)
!753 = !DILocation(line: 372, column: 9, scope: !750)
!754 = !DILocation(line: 374, column: 29, scope: !712)
!755 = !DILocation(line: 374, column: 35, scope: !712)
!756 = !DILocation(line: 374, column: 9, scope: !712)
!757 = !DILocation(line: 377, column: 5, scope: !682)
!758 = !DILocalVariable(name: "i", scope: !759, file: !2, line: 379, type: !38)
!759 = distinct !DILexicalBlock(scope: !682, file: !2, line: 379, column: 5)
!760 = !DILocation(line: 379, column: 14, scope: !759)
!761 = !DILocation(line: 379, column: 10, scope: !759)
!762 = !DILocation(line: 379, column: 21, scope: !763)
!763 = distinct !DILexicalBlock(scope: !759, file: !2, line: 379, column: 5)
!764 = !DILocation(line: 379, column: 36, scope: !763)
!765 = !DILocation(line: 379, column: 23, scope: !763)
!766 = !DILocation(line: 379, column: 5, scope: !759)
!767 = !DILocalVariable(name: "flag", scope: !768, file: !2, line: 380, type: !713)
!768 = distinct !DILexicalBlock(scope: !763, file: !2, line: 379, column: 54)
!769 = !DILocation(line: 380, column: 26, scope: !768)
!770 = !DILocation(line: 380, column: 51, scope: !768)
!771 = !DILocation(line: 380, column: 34, scope: !768)
!772 = !DILocation(line: 381, column: 13, scope: !773)
!773 = distinct !DILexicalBlock(scope: !768, file: !2, line: 381, column: 13)
!774 = !DILocation(line: 381, column: 19, scope: !773)
!775 = !DILocation(line: 381, column: 28, scope: !773)
!776 = !DILocation(line: 381, column: 32, scope: !773)
!777 = !DILocation(line: 381, column: 38, scope: !773)
!778 = !DILocation(line: 381, column: 47, scope: !773)
!779 = !DILocation(line: 381, column: 51, scope: !773)
!780 = !DILocation(line: 381, column: 57, scope: !773)
!781 = !DILocation(line: 382, column: 68, scope: !782)
!782 = distinct !DILexicalBlock(scope: !773, file: !2, line: 381, column: 64)
!783 = !DILocation(line: 382, column: 74, scope: !782)
!784 = !DILocation(line: 382, column: 13, scope: !782)
!785 = !DILocation(line: 383, column: 9, scope: !782)
!786 = !DILocation(line: 384, column: 5, scope: !768)
!787 = !DILocation(line: 379, column: 50, scope: !763)
!788 = !DILocation(line: 379, column: 5, scope: !763)
!789 = distinct !{!789, !766, !790, !740}
!790 = !DILocation(line: 384, column: 5, scope: !759)
!791 = !DILocation(line: 386, column: 20, scope: !792)
!792 = distinct !DILexicalBlock(scope: !682, file: !2, line: 386, column: 9)
!793 = !DILocation(line: 386, column: 33, scope: !792)
!794 = !DILocation(line: 387, column: 9, scope: !795)
!795 = distinct !DILexicalBlock(scope: !792, file: !2, line: 386, column: 38)
!796 = !DILocation(line: 390, column: 5, scope: !682)
!797 = !DILocation(line: 391, column: 1, scope: !682)
!798 = distinct !DISubprogram(name: "_kgflags_consume_arg", scope: !2, file: !2, line: 699, type: !799, scopeLine: 699, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !9, retainedNodes: !343)
!799 = !DISubroutineType(types: !800)
!800 = !{!259}
!801 = !DILocation(line: 700, column: 20, scope: !802)
!802 = distinct !DILexicalBlock(scope: !798, file: !2, line: 700, column: 9)
!803 = !DILocation(line: 700, column: 45, scope: !802)
!804 = !DILocation(line: 700, column: 31, scope: !802)
!805 = !DILocation(line: 701, column: 9, scope: !806)
!806 = distinct !DILexicalBlock(scope: !802, file: !2, line: 700, column: 51)
!807 = !DILocalVariable(name: "res", scope: !798, file: !2, line: 703, type: !259)
!808 = !DILocation(line: 703, column: 17, scope: !798)
!809 = !DILocation(line: 703, column: 34, scope: !798)
!810 = !DILocation(line: 703, column: 50, scope: !798)
!811 = !DILocation(line: 703, column: 23, scope: !798)
!812 = !DILocation(line: 704, column: 26, scope: !798)
!813 = !DILocation(line: 705, column: 12, scope: !798)
!814 = !DILocation(line: 705, column: 5, scope: !798)
!815 = !DILocation(line: 706, column: 1, scope: !798)
!816 = distinct !DISubprogram(name: "_kgflags_is_flag", scope: !2, file: !2, line: 568, type: !817, scopeLine: 568, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !9, retainedNodes: !343)
!817 = !DISubroutineType(types: !818)
!818 = !{!267, !259}
!819 = !DILocalVariable(name: "arg", arg: 1, scope: !816, file: !2, line: 568, type: !259)
!820 = !DILocation(line: 568, column: 42, scope: !816)
!821 = !DILocation(line: 569, column: 35, scope: !816)
!822 = !DILocation(line: 569, column: 12, scope: !816)
!823 = !DILocation(line: 569, column: 40, scope: !816)
!824 = !DILocation(line: 569, column: 5, scope: !816)
!825 = distinct !DISubprogram(name: "_kgflags_get_flag_name", scope: !2, file: !2, line: 572, type: !826, scopeLine: 572, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !9, retainedNodes: !343)
!826 = !DISubroutineType(types: !827)
!827 = !{!259, !259}
!828 = !DILocalVariable(name: "arg", arg: 1, scope: !825, file: !2, line: 572, type: !259)
!829 = !DILocation(line: 572, column: 55, scope: !825)
!830 = !DILocalVariable(name: "prefix_len", scope: !825, file: !2, line: 573, type: !831)
!831 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!832 = !DILocation(line: 573, column: 19, scope: !825)
!833 = !DILocation(line: 573, column: 50, scope: !825)
!834 = !DILocation(line: 573, column: 32, scope: !825)
!835 = !DILocation(line: 574, column: 16, scope: !836)
!836 = distinct !DILexicalBlock(scope: !825, file: !2, line: 574, column: 9)
!837 = !DILocation(line: 574, column: 9, scope: !836)
!838 = !DILocation(line: 574, column: 23, scope: !836)
!839 = !DILocation(line: 574, column: 21, scope: !836)
!840 = !DILocation(line: 575, column: 9, scope: !841)
!841 = distinct !DILexicalBlock(scope: !836, file: !2, line: 574, column: 35)
!842 = !DILocation(line: 577, column: 17, scope: !843)
!843 = distinct !DILexicalBlock(scope: !825, file: !2, line: 577, column: 9)
!844 = !DILocation(line: 577, column: 33, scope: !843)
!845 = !DILocation(line: 577, column: 46, scope: !843)
!846 = !DILocation(line: 577, column: 9, scope: !843)
!847 = !DILocation(line: 577, column: 58, scope: !843)
!848 = !DILocation(line: 578, column: 9, scope: !849)
!849 = distinct !DILexicalBlock(scope: !843, file: !2, line: 577, column: 64)
!850 = !DILocation(line: 580, column: 12, scope: !825)
!851 = !DILocation(line: 580, column: 18, scope: !825)
!852 = !DILocation(line: 580, column: 16, scope: !825)
!853 = !DILocation(line: 580, column: 5, scope: !825)
!854 = !DILocation(line: 581, column: 1, scope: !825)
!855 = distinct !DISubprogram(name: "_kgflags_get_flag", scope: !2, file: !2, line: 596, type: !856, scopeLine: 596, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !9, retainedNodes: !343)
!856 = !DISubroutineType(types: !857)
!857 = !{!713, !259, !277}
!858 = !DILocalVariable(name: "name", arg: 1, scope: !855, file: !2, line: 596, type: !259)
!859 = !DILocation(line: 596, column: 55, scope: !855)
!860 = !DILocalVariable(name: "out_prefix_no", arg: 2, scope: !855, file: !2, line: 596, type: !277)
!861 = !DILocation(line: 596, column: 67, scope: !855)
!862 = !DILocation(line: 597, column: 9, scope: !863)
!863 = distinct !DILexicalBlock(scope: !855, file: !2, line: 597, column: 9)
!864 = !DILocation(line: 598, column: 10, scope: !865)
!865 = distinct !DILexicalBlock(scope: !863, file: !2, line: 597, column: 24)
!866 = !DILocation(line: 598, column: 24, scope: !865)
!867 = !DILocation(line: 599, column: 5, scope: !865)
!868 = !DILocalVariable(name: "i", scope: !869, file: !2, line: 600, type: !38)
!869 = distinct !DILexicalBlock(scope: !855, file: !2, line: 600, column: 5)
!870 = !DILocation(line: 600, column: 14, scope: !869)
!871 = !DILocation(line: 600, column: 10, scope: !869)
!872 = !DILocation(line: 600, column: 21, scope: !873)
!873 = distinct !DILexicalBlock(scope: !869, file: !2, line: 600, column: 5)
!874 = !DILocation(line: 600, column: 36, scope: !873)
!875 = !DILocation(line: 600, column: 23, scope: !873)
!876 = !DILocation(line: 600, column: 5, scope: !869)
!877 = !DILocalVariable(name: "flag", scope: !878, file: !2, line: 601, type: !713)
!878 = distinct !DILexicalBlock(scope: !873, file: !2, line: 600, column: 54)
!879 = !DILocation(line: 601, column: 26, scope: !878)
!880 = !DILocation(line: 601, column: 51, scope: !878)
!881 = !DILocation(line: 601, column: 34, scope: !878)
!882 = !DILocation(line: 602, column: 20, scope: !883)
!883 = distinct !DILexicalBlock(scope: !878, file: !2, line: 602, column: 13)
!884 = !DILocation(line: 602, column: 26, scope: !883)
!885 = !DILocation(line: 602, column: 32, scope: !883)
!886 = !DILocation(line: 602, column: 13, scope: !883)
!887 = !DILocation(line: 602, column: 38, scope: !883)
!888 = !DILocation(line: 603, column: 20, scope: !889)
!889 = distinct !DILexicalBlock(scope: !883, file: !2, line: 602, column: 44)
!890 = !DILocation(line: 603, column: 13, scope: !889)
!891 = !DILocation(line: 605, column: 13, scope: !892)
!892 = distinct !DILexicalBlock(scope: !878, file: !2, line: 605, column: 13)
!893 = !DILocation(line: 605, column: 19, scope: !892)
!894 = !DILocation(line: 605, column: 24, scope: !892)
!895 = !DILocation(line: 605, column: 50, scope: !892)
!896 = !DILocation(line: 605, column: 60, scope: !892)
!897 = !DILocation(line: 605, column: 53, scope: !892)
!898 = !DILocation(line: 605, column: 76, scope: !892)
!899 = !DILocation(line: 605, column: 73, scope: !892)
!900 = !DILocation(line: 606, column: 24, scope: !901)
!901 = distinct !DILexicalBlock(scope: !902, file: !2, line: 606, column: 17)
!902 = distinct !DILexicalBlock(scope: !892, file: !2, line: 605, column: 82)
!903 = !DILocation(line: 606, column: 29, scope: !901)
!904 = !DILocation(line: 606, column: 34, scope: !901)
!905 = !DILocation(line: 606, column: 40, scope: !901)
!906 = !DILocation(line: 606, column: 17, scope: !901)
!907 = !DILocation(line: 606, column: 46, scope: !901)
!908 = !DILocation(line: 607, column: 21, scope: !909)
!909 = distinct !DILexicalBlock(scope: !910, file: !2, line: 607, column: 21)
!910 = distinct !DILexicalBlock(scope: !901, file: !2, line: 606, column: 52)
!911 = !DILocation(line: 608, column: 22, scope: !912)
!912 = distinct !DILexicalBlock(scope: !909, file: !2, line: 607, column: 36)
!913 = !DILocation(line: 608, column: 36, scope: !912)
!914 = !DILocation(line: 609, column: 17, scope: !912)
!915 = !DILocation(line: 610, column: 24, scope: !910)
!916 = !DILocation(line: 610, column: 17, scope: !910)
!917 = !DILocation(line: 612, column: 9, scope: !902)
!918 = !DILocation(line: 613, column: 5, scope: !878)
!919 = !DILocation(line: 600, column: 50, scope: !873)
!920 = !DILocation(line: 600, column: 5, scope: !873)
!921 = distinct !{!921, !876, !922, !740}
!922 = !DILocation(line: 613, column: 5, scope: !869)
!923 = !DILocation(line: 614, column: 5, scope: !855)
!924 = !DILocation(line: 615, column: 1, scope: !855)
!925 = distinct !DISubprogram(name: "_kgflags_add_non_flag_arg", scope: !2, file: !2, line: 689, type: !817, scopeLine: 689, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !9, retainedNodes: !343)
!926 = !DILocalVariable(name: "arg", arg: 1, scope: !925, file: !2, line: 689, type: !259)
!927 = !DILocation(line: 689, column: 51, scope: !925)
!928 = !DILocation(line: 690, column: 20, scope: !929)
!929 = distinct !DILexicalBlock(scope: !925, file: !2, line: 690, column: 9)
!930 = !DILocation(line: 690, column: 35, scope: !929)
!931 = !DILocation(line: 691, column: 9, scope: !932)
!932 = distinct !DILexicalBlock(scope: !929, file: !2, line: 690, column: 65)
!933 = !DILocation(line: 692, column: 9, scope: !932)
!934 = !DILocation(line: 694, column: 59, scope: !925)
!935 = !DILocation(line: 694, column: 41, scope: !925)
!936 = !DILocation(line: 694, column: 5, scope: !925)
!937 = !DILocation(line: 694, column: 57, scope: !925)
!938 = !DILocation(line: 695, column: 30, scope: !925)
!939 = !DILocation(line: 696, column: 5, scope: !925)
!940 = !DILocation(line: 697, column: 1, scope: !925)
!941 = distinct !DISubprogram(name: "_kgflags_parse_flag", scope: !2, file: !2, line: 715, type: !942, scopeLine: 715, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !9, retainedNodes: !343)
!942 = !DISubroutineType(types: !943)
!943 = !{null, !713, !267}
!944 = !DILocalVariable(name: "flag", arg: 1, scope: !941, file: !2, line: 715, type: !713)
!945 = !DILocation(line: 715, column: 50, scope: !941)
!946 = !DILocalVariable(name: "prefix_no", arg: 2, scope: !941, file: !2, line: 715, type: !267)
!947 = !DILocation(line: 715, column: 61, scope: !941)
!948 = !DILocation(line: 716, column: 13, scope: !941)
!949 = !DILocation(line: 716, column: 19, scope: !941)
!950 = !DILocation(line: 716, column: 5, scope: !941)
!951 = !DILocalVariable(name: "val", scope: !952, file: !2, line: 718, type: !259)
!952 = distinct !DILexicalBlock(scope: !953, file: !2, line: 717, column: 40)
!953 = distinct !DILexicalBlock(scope: !941, file: !2, line: 716, column: 25)
!954 = !DILocation(line: 718, column: 25, scope: !952)
!955 = !DILocation(line: 718, column: 31, scope: !952)
!956 = !DILocation(line: 719, column: 18, scope: !957)
!957 = distinct !DILexicalBlock(scope: !952, file: !2, line: 719, column: 17)
!958 = !DILocation(line: 719, column: 17, scope: !957)
!959 = !DILocation(line: 720, column: 17, scope: !960)
!960 = distinct !DILexicalBlock(scope: !957, file: !2, line: 719, column: 23)
!961 = !DILocation(line: 720, column: 23, scope: !960)
!962 = !DILocation(line: 720, column: 29, scope: !960)
!963 = !DILocation(line: 721, column: 70, scope: !960)
!964 = !DILocation(line: 721, column: 76, scope: !960)
!965 = !DILocation(line: 721, column: 17, scope: !960)
!966 = !DILocation(line: 722, column: 17, scope: !960)
!967 = !DILocation(line: 724, column: 42, scope: !952)
!968 = !DILocation(line: 724, column: 14, scope: !952)
!969 = !DILocation(line: 724, column: 20, scope: !952)
!970 = !DILocation(line: 724, column: 27, scope: !952)
!971 = !DILocation(line: 724, column: 40, scope: !952)
!972 = !DILocation(line: 725, column: 13, scope: !952)
!973 = !DILocation(line: 725, column: 19, scope: !952)
!974 = !DILocation(line: 725, column: 28, scope: !952)
!975 = !DILocation(line: 726, column: 13, scope: !952)
!976 = !DILocation(line: 729, column: 41, scope: !977)
!977 = distinct !DILexicalBlock(scope: !953, file: !2, line: 728, column: 38)
!978 = !DILocation(line: 729, column: 40, scope: !977)
!979 = !DILocation(line: 729, column: 14, scope: !977)
!980 = !DILocation(line: 729, column: 20, scope: !977)
!981 = !DILocation(line: 729, column: 27, scope: !977)
!982 = !DILocation(line: 729, column: 38, scope: !977)
!983 = !DILocation(line: 730, column: 13, scope: !977)
!984 = !DILocation(line: 730, column: 19, scope: !977)
!985 = !DILocation(line: 730, column: 28, scope: !977)
!986 = !DILocation(line: 731, column: 13, scope: !977)
!987 = !DILocalVariable(name: "val", scope: !988, file: !2, line: 734, type: !259)
!988 = distinct !DILexicalBlock(scope: !953, file: !2, line: 733, column: 37)
!989 = !DILocation(line: 734, column: 25, scope: !988)
!990 = !DILocation(line: 734, column: 31, scope: !988)
!991 = !DILocation(line: 735, column: 18, scope: !992)
!992 = distinct !DILexicalBlock(scope: !988, file: !2, line: 735, column: 17)
!993 = !DILocation(line: 735, column: 17, scope: !992)
!994 = !DILocation(line: 736, column: 17, scope: !995)
!995 = distinct !DILexicalBlock(scope: !992, file: !2, line: 735, column: 23)
!996 = !DILocation(line: 736, column: 23, scope: !995)
!997 = !DILocation(line: 736, column: 29, scope: !995)
!998 = !DILocation(line: 737, column: 70, scope: !995)
!999 = !DILocation(line: 737, column: 76, scope: !995)
!1000 = !DILocation(line: 737, column: 17, scope: !995)
!1001 = !DILocation(line: 738, column: 17, scope: !995)
!1002 = !DILocalVariable(name: "ok", scope: !988, file: !2, line: 740, type: !267)
!1003 = !DILocation(line: 740, column: 18, scope: !988)
!1004 = !DILocalVariable(name: "int_val", scope: !988, file: !2, line: 741, type: !38)
!1005 = !DILocation(line: 741, column: 17, scope: !988)
!1006 = !DILocation(line: 741, column: 46, scope: !988)
!1007 = !DILocation(line: 741, column: 27, scope: !988)
!1008 = !DILocation(line: 742, column: 18, scope: !1009)
!1009 = distinct !DILexicalBlock(scope: !988, file: !2, line: 742, column: 17)
!1010 = !DILocation(line: 742, column: 17, scope: !1009)
!1011 = !DILocation(line: 743, column: 17, scope: !1012)
!1012 = distinct !DILexicalBlock(scope: !1009, file: !2, line: 742, column: 22)
!1013 = !DILocation(line: 743, column: 23, scope: !1012)
!1014 = !DILocation(line: 743, column: 29, scope: !1012)
!1015 = !DILocation(line: 744, column: 68, scope: !1012)
!1016 = !DILocation(line: 744, column: 74, scope: !1012)
!1017 = !DILocation(line: 744, column: 80, scope: !1012)
!1018 = !DILocation(line: 744, column: 17, scope: !1012)
!1019 = !DILocation(line: 745, column: 17, scope: !1012)
!1020 = !DILocation(line: 747, column: 39, scope: !988)
!1021 = !DILocation(line: 747, column: 14, scope: !988)
!1022 = !DILocation(line: 747, column: 20, scope: !988)
!1023 = !DILocation(line: 747, column: 27, scope: !988)
!1024 = !DILocation(line: 747, column: 37, scope: !988)
!1025 = !DILocation(line: 748, column: 13, scope: !988)
!1026 = !DILocation(line: 748, column: 19, scope: !988)
!1027 = !DILocation(line: 748, column: 28, scope: !988)
!1028 = !DILocation(line: 749, column: 13, scope: !988)
!1029 = !DILocalVariable(name: "val", scope: !1030, file: !2, line: 752, type: !259)
!1030 = distinct !DILexicalBlock(scope: !953, file: !2, line: 751, column: 40)
!1031 = !DILocation(line: 752, column: 25, scope: !1030)
!1032 = !DILocation(line: 752, column: 31, scope: !1030)
!1033 = !DILocation(line: 753, column: 18, scope: !1034)
!1034 = distinct !DILexicalBlock(scope: !1030, file: !2, line: 753, column: 17)
!1035 = !DILocation(line: 753, column: 17, scope: !1034)
!1036 = !DILocation(line: 754, column: 17, scope: !1037)
!1037 = distinct !DILexicalBlock(scope: !1034, file: !2, line: 753, column: 23)
!1038 = !DILocation(line: 754, column: 23, scope: !1037)
!1039 = !DILocation(line: 754, column: 29, scope: !1037)
!1040 = !DILocation(line: 755, column: 70, scope: !1037)
!1041 = !DILocation(line: 755, column: 76, scope: !1037)
!1042 = !DILocation(line: 755, column: 17, scope: !1037)
!1043 = !DILocation(line: 756, column: 17, scope: !1037)
!1044 = !DILocalVariable(name: "ok", scope: !1030, file: !2, line: 758, type: !267)
!1045 = !DILocation(line: 758, column: 18, scope: !1030)
!1046 = !DILocalVariable(name: "double_val", scope: !1030, file: !2, line: 759, type: !270)
!1047 = !DILocation(line: 759, column: 20, scope: !1030)
!1048 = !DILocation(line: 759, column: 55, scope: !1030)
!1049 = !DILocation(line: 759, column: 33, scope: !1030)
!1050 = !DILocation(line: 760, column: 18, scope: !1051)
!1051 = distinct !DILexicalBlock(scope: !1030, file: !2, line: 760, column: 17)
!1052 = !DILocation(line: 760, column: 17, scope: !1051)
!1053 = !DILocation(line: 761, column: 17, scope: !1054)
!1054 = distinct !DILexicalBlock(scope: !1051, file: !2, line: 760, column: 22)
!1055 = !DILocation(line: 761, column: 23, scope: !1054)
!1056 = !DILocation(line: 761, column: 29, scope: !1054)
!1057 = !DILocation(line: 762, column: 71, scope: !1054)
!1058 = !DILocation(line: 762, column: 77, scope: !1054)
!1059 = !DILocation(line: 762, column: 83, scope: !1054)
!1060 = !DILocation(line: 762, column: 17, scope: !1054)
!1061 = !DILocation(line: 763, column: 17, scope: !1054)
!1062 = !DILocation(line: 765, column: 42, scope: !1030)
!1063 = !DILocation(line: 765, column: 14, scope: !1030)
!1064 = !DILocation(line: 765, column: 20, scope: !1030)
!1065 = !DILocation(line: 765, column: 27, scope: !1030)
!1066 = !DILocation(line: 765, column: 40, scope: !1030)
!1067 = !DILocation(line: 766, column: 13, scope: !1030)
!1068 = !DILocation(line: 766, column: 19, scope: !1030)
!1069 = !DILocation(line: 766, column: 28, scope: !1030)
!1070 = !DILocation(line: 767, column: 13, scope: !1030)
!1071 = !DILocalVariable(name: "initial_cursor", scope: !1072, file: !2, line: 770, type: !38)
!1072 = distinct !DILexicalBlock(scope: !953, file: !2, line: 769, column: 46)
!1073 = !DILocation(line: 770, column: 17, scope: !1072)
!1074 = !DILocation(line: 770, column: 45, scope: !1072)
!1075 = !DILocalVariable(name: "count", scope: !1072, file: !2, line: 771, type: !38)
!1076 = !DILocation(line: 771, column: 17, scope: !1072)
!1077 = !DILocation(line: 772, column: 13, scope: !1072)
!1078 = !DILocalVariable(name: "val", scope: !1079, file: !2, line: 773, type: !259)
!1079 = distinct !DILexicalBlock(scope: !1072, file: !2, line: 772, column: 26)
!1080 = !DILocation(line: 773, column: 29, scope: !1079)
!1081 = !DILocation(line: 773, column: 35, scope: !1079)
!1082 = !DILocation(line: 774, column: 21, scope: !1083)
!1083 = distinct !DILexicalBlock(scope: !1079, file: !2, line: 774, column: 21)
!1084 = !DILocation(line: 774, column: 25, scope: !1083)
!1085 = !DILocation(line: 774, column: 33, scope: !1083)
!1086 = !DILocation(line: 774, column: 53, scope: !1083)
!1087 = !DILocation(line: 774, column: 36, scope: !1083)
!1088 = !DILocation(line: 775, column: 21, scope: !1089)
!1089 = distinct !DILexicalBlock(scope: !1083, file: !2, line: 774, column: 59)
!1090 = !DILocation(line: 777, column: 17, scope: !1079)
!1091 = !DILocation(line: 778, column: 22, scope: !1079)
!1092 = distinct !{!1092, !1077, !1093}
!1093 = !DILocation(line: 779, column: 13, scope: !1072)
!1094 = !DILocalVariable(name: "arr", scope: !1072, file: !2, line: 780, type: !283)
!1095 = !DILocation(line: 780, column: 37, scope: !1072)
!1096 = !DILocation(line: 780, column: 43, scope: !1072)
!1097 = !DILocation(line: 780, column: 49, scope: !1072)
!1098 = !DILocation(line: 780, column: 56, scope: !1072)
!1099 = !DILocation(line: 781, column: 38, scope: !1072)
!1100 = !DILocation(line: 781, column: 45, scope: !1072)
!1101 = !DILocation(line: 781, column: 43, scope: !1072)
!1102 = !DILocation(line: 781, column: 13, scope: !1072)
!1103 = !DILocation(line: 781, column: 18, scope: !1072)
!1104 = !DILocation(line: 781, column: 25, scope: !1072)
!1105 = !DILocation(line: 782, column: 27, scope: !1072)
!1106 = !DILocation(line: 782, column: 13, scope: !1072)
!1107 = !DILocation(line: 782, column: 18, scope: !1072)
!1108 = !DILocation(line: 782, column: 25, scope: !1072)
!1109 = !DILocation(line: 783, column: 13, scope: !1072)
!1110 = !DILocation(line: 783, column: 19, scope: !1072)
!1111 = !DILocation(line: 783, column: 28, scope: !1072)
!1112 = !DILocation(line: 784, column: 13, scope: !1072)
!1113 = !DILocalVariable(name: "initial_cursor", scope: !1114, file: !2, line: 787, type: !38)
!1114 = distinct !DILexicalBlock(scope: !953, file: !2, line: 786, column: 43)
!1115 = !DILocation(line: 787, column: 17, scope: !1114)
!1116 = !DILocation(line: 787, column: 45, scope: !1114)
!1117 = !DILocalVariable(name: "count", scope: !1114, file: !2, line: 788, type: !38)
!1118 = !DILocation(line: 788, column: 17, scope: !1114)
!1119 = !DILocalVariable(name: "all_args_ok", scope: !1114, file: !2, line: 789, type: !267)
!1120 = !DILocation(line: 789, column: 18, scope: !1114)
!1121 = !DILocation(line: 790, column: 13, scope: !1114)
!1122 = !DILocalVariable(name: "val", scope: !1123, file: !2, line: 791, type: !259)
!1123 = distinct !DILexicalBlock(scope: !1114, file: !2, line: 790, column: 26)
!1124 = !DILocation(line: 791, column: 29, scope: !1123)
!1125 = !DILocation(line: 791, column: 35, scope: !1123)
!1126 = !DILocation(line: 792, column: 21, scope: !1127)
!1127 = distinct !DILexicalBlock(scope: !1123, file: !2, line: 792, column: 21)
!1128 = !DILocation(line: 792, column: 25, scope: !1127)
!1129 = !DILocation(line: 792, column: 33, scope: !1127)
!1130 = !DILocation(line: 792, column: 53, scope: !1127)
!1131 = !DILocation(line: 792, column: 36, scope: !1127)
!1132 = !DILocation(line: 793, column: 21, scope: !1133)
!1133 = distinct !DILexicalBlock(scope: !1127, file: !2, line: 792, column: 59)
!1134 = !DILocation(line: 795, column: 17, scope: !1123)
!1135 = !DILocalVariable(name: "ok", scope: !1123, file: !2, line: 796, type: !267)
!1136 = !DILocation(line: 796, column: 22, scope: !1123)
!1137 = !DILocation(line: 797, column: 36, scope: !1123)
!1138 = !DILocation(line: 797, column: 17, scope: !1123)
!1139 = !DILocation(line: 798, column: 22, scope: !1140)
!1140 = distinct !DILexicalBlock(scope: !1123, file: !2, line: 798, column: 21)
!1141 = !DILocation(line: 798, column: 21, scope: !1140)
!1142 = !DILocation(line: 799, column: 21, scope: !1143)
!1143 = distinct !DILexicalBlock(scope: !1140, file: !2, line: 798, column: 26)
!1144 = !DILocation(line: 799, column: 27, scope: !1143)
!1145 = !DILocation(line: 799, column: 33, scope: !1143)
!1146 = !DILocation(line: 800, column: 72, scope: !1143)
!1147 = !DILocation(line: 800, column: 78, scope: !1143)
!1148 = !DILocation(line: 800, column: 84, scope: !1143)
!1149 = !DILocation(line: 800, column: 21, scope: !1143)
!1150 = !DILocation(line: 801, column: 33, scope: !1143)
!1151 = !DILocation(line: 802, column: 17, scope: !1143)
!1152 = !DILocation(line: 803, column: 22, scope: !1123)
!1153 = distinct !{!1153, !1121, !1154}
!1154 = !DILocation(line: 804, column: 13, scope: !1114)
!1155 = !DILocalVariable(name: "arr", scope: !1114, file: !2, line: 805, type: !292)
!1156 = !DILocation(line: 805, column: 34, scope: !1114)
!1157 = !DILocation(line: 805, column: 40, scope: !1114)
!1158 = !DILocation(line: 805, column: 46, scope: !1114)
!1159 = !DILocation(line: 805, column: 53, scope: !1114)
!1160 = !DILocation(line: 806, column: 17, scope: !1161)
!1161 = distinct !DILexicalBlock(scope: !1114, file: !2, line: 806, column: 17)
!1162 = !DILocation(line: 807, column: 42, scope: !1163)
!1163 = distinct !DILexicalBlock(scope: !1161, file: !2, line: 806, column: 30)
!1164 = !DILocation(line: 807, column: 49, scope: !1163)
!1165 = !DILocation(line: 807, column: 47, scope: !1163)
!1166 = !DILocation(line: 807, column: 17, scope: !1163)
!1167 = !DILocation(line: 807, column: 22, scope: !1163)
!1168 = !DILocation(line: 807, column: 29, scope: !1163)
!1169 = !DILocation(line: 808, column: 31, scope: !1163)
!1170 = !DILocation(line: 808, column: 17, scope: !1163)
!1171 = !DILocation(line: 808, column: 22, scope: !1163)
!1172 = !DILocation(line: 808, column: 29, scope: !1163)
!1173 = !DILocation(line: 809, column: 13, scope: !1163)
!1174 = !DILocation(line: 810, column: 13, scope: !1114)
!1175 = !DILocation(line: 810, column: 19, scope: !1114)
!1176 = !DILocation(line: 810, column: 28, scope: !1114)
!1177 = !DILocation(line: 811, column: 13, scope: !1114)
!1178 = !DILocalVariable(name: "initial_cursor", scope: !1179, file: !2, line: 814, type: !38)
!1179 = distinct !DILexicalBlock(scope: !953, file: !2, line: 813, column: 46)
!1180 = !DILocation(line: 814, column: 17, scope: !1179)
!1181 = !DILocation(line: 814, column: 45, scope: !1179)
!1182 = !DILocalVariable(name: "count", scope: !1179, file: !2, line: 815, type: !38)
!1183 = !DILocation(line: 815, column: 17, scope: !1179)
!1184 = !DILocalVariable(name: "all_args_ok", scope: !1179, file: !2, line: 816, type: !267)
!1185 = !DILocation(line: 816, column: 18, scope: !1179)
!1186 = !DILocation(line: 817, column: 13, scope: !1179)
!1187 = !DILocalVariable(name: "val", scope: !1188, file: !2, line: 818, type: !259)
!1188 = distinct !DILexicalBlock(scope: !1179, file: !2, line: 817, column: 26)
!1189 = !DILocation(line: 818, column: 29, scope: !1188)
!1190 = !DILocation(line: 818, column: 35, scope: !1188)
!1191 = !DILocation(line: 819, column: 21, scope: !1192)
!1192 = distinct !DILexicalBlock(scope: !1188, file: !2, line: 819, column: 21)
!1193 = !DILocation(line: 819, column: 25, scope: !1192)
!1194 = !DILocation(line: 819, column: 33, scope: !1192)
!1195 = !DILocation(line: 819, column: 53, scope: !1192)
!1196 = !DILocation(line: 819, column: 36, scope: !1192)
!1197 = !DILocation(line: 820, column: 21, scope: !1198)
!1198 = distinct !DILexicalBlock(scope: !1192, file: !2, line: 819, column: 59)
!1199 = !DILocation(line: 822, column: 17, scope: !1188)
!1200 = !DILocalVariable(name: "ok", scope: !1188, file: !2, line: 823, type: !267)
!1201 = !DILocation(line: 823, column: 22, scope: !1188)
!1202 = !DILocation(line: 824, column: 39, scope: !1188)
!1203 = !DILocation(line: 824, column: 17, scope: !1188)
!1204 = !DILocation(line: 825, column: 22, scope: !1205)
!1205 = distinct !DILexicalBlock(scope: !1188, file: !2, line: 825, column: 21)
!1206 = !DILocation(line: 825, column: 21, scope: !1205)
!1207 = !DILocation(line: 826, column: 21, scope: !1208)
!1208 = distinct !DILexicalBlock(scope: !1205, file: !2, line: 825, column: 26)
!1209 = !DILocation(line: 826, column: 27, scope: !1208)
!1210 = !DILocation(line: 826, column: 33, scope: !1208)
!1211 = !DILocation(line: 827, column: 75, scope: !1208)
!1212 = !DILocation(line: 827, column: 81, scope: !1208)
!1213 = !DILocation(line: 827, column: 87, scope: !1208)
!1214 = !DILocation(line: 827, column: 21, scope: !1208)
!1215 = !DILocation(line: 828, column: 33, scope: !1208)
!1216 = !DILocation(line: 829, column: 17, scope: !1208)
!1217 = !DILocation(line: 830, column: 22, scope: !1188)
!1218 = distinct !{!1218, !1186, !1219}
!1219 = !DILocation(line: 831, column: 13, scope: !1179)
!1220 = !DILocalVariable(name: "arr", scope: !1179, file: !2, line: 832, type: !299)
!1221 = !DILocation(line: 832, column: 37, scope: !1179)
!1222 = !DILocation(line: 832, column: 43, scope: !1179)
!1223 = !DILocation(line: 832, column: 49, scope: !1179)
!1224 = !DILocation(line: 832, column: 56, scope: !1179)
!1225 = !DILocation(line: 833, column: 17, scope: !1226)
!1226 = distinct !DILexicalBlock(scope: !1179, file: !2, line: 833, column: 17)
!1227 = !DILocation(line: 834, column: 42, scope: !1228)
!1228 = distinct !DILexicalBlock(scope: !1226, file: !2, line: 833, column: 30)
!1229 = !DILocation(line: 834, column: 49, scope: !1228)
!1230 = !DILocation(line: 834, column: 47, scope: !1228)
!1231 = !DILocation(line: 834, column: 17, scope: !1228)
!1232 = !DILocation(line: 834, column: 22, scope: !1228)
!1233 = !DILocation(line: 834, column: 29, scope: !1228)
!1234 = !DILocation(line: 835, column: 31, scope: !1228)
!1235 = !DILocation(line: 835, column: 17, scope: !1228)
!1236 = !DILocation(line: 835, column: 22, scope: !1228)
!1237 = !DILocation(line: 835, column: 29, scope: !1228)
!1238 = !DILocation(line: 836, column: 13, scope: !1228)
!1239 = !DILocation(line: 837, column: 13, scope: !1179)
!1240 = !DILocation(line: 837, column: 19, scope: !1179)
!1241 = !DILocation(line: 837, column: 28, scope: !1179)
!1242 = !DILocation(line: 838, column: 13, scope: !1179)
!1243 = !DILocation(line: 841, column: 13, scope: !953)
!1244 = !DILocation(line: 843, column: 1, scope: !941)
!1245 = distinct !DISubprogram(name: "_kgflags_assign_default_values", scope: !2, file: !2, line: 656, type: !1246, scopeLine: 656, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !9, retainedNodes: !343)
!1246 = !DISubroutineType(types: !1247)
!1247 = !{null}
!1248 = !DILocalVariable(name: "i", scope: !1249, file: !2, line: 657, type: !38)
!1249 = distinct !DILexicalBlock(scope: !1245, file: !2, line: 657, column: 5)
!1250 = !DILocation(line: 657, column: 14, scope: !1249)
!1251 = !DILocation(line: 657, column: 10, scope: !1249)
!1252 = !DILocation(line: 657, column: 21, scope: !1253)
!1253 = distinct !DILexicalBlock(scope: !1249, file: !2, line: 657, column: 5)
!1254 = !DILocation(line: 657, column: 36, scope: !1253)
!1255 = !DILocation(line: 657, column: 23, scope: !1253)
!1256 = !DILocation(line: 657, column: 5, scope: !1249)
!1257 = !DILocalVariable(name: "flag", scope: !1258, file: !2, line: 658, type: !713)
!1258 = distinct !DILexicalBlock(scope: !1253, file: !2, line: 657, column: 54)
!1259 = !DILocation(line: 658, column: 26, scope: !1258)
!1260 = !DILocation(line: 658, column: 51, scope: !1258)
!1261 = !DILocation(line: 658, column: 34, scope: !1258)
!1262 = !DILocation(line: 659, column: 13, scope: !1263)
!1263 = distinct !DILexicalBlock(scope: !1258, file: !2, line: 659, column: 13)
!1264 = !DILocation(line: 659, column: 19, scope: !1263)
!1265 = !DILocation(line: 659, column: 28, scope: !1263)
!1266 = !DILocation(line: 659, column: 31, scope: !1263)
!1267 = !DILocation(line: 659, column: 37, scope: !1263)
!1268 = !DILocation(line: 660, column: 13, scope: !1269)
!1269 = distinct !DILexicalBlock(scope: !1263, file: !2, line: 659, column: 47)
!1270 = !DILocation(line: 662, column: 17, scope: !1258)
!1271 = !DILocation(line: 662, column: 23, scope: !1258)
!1272 = !DILocation(line: 662, column: 9, scope: !1258)
!1273 = !DILocation(line: 664, column: 46, scope: !1274)
!1274 = distinct !DILexicalBlock(scope: !1275, file: !2, line: 663, column: 44)
!1275 = distinct !DILexicalBlock(scope: !1258, file: !2, line: 662, column: 29)
!1276 = !DILocation(line: 664, column: 52, scope: !1274)
!1277 = !DILocation(line: 664, column: 66, scope: !1274)
!1278 = !DILocation(line: 664, column: 18, scope: !1274)
!1279 = !DILocation(line: 664, column: 24, scope: !1274)
!1280 = !DILocation(line: 664, column: 31, scope: !1274)
!1281 = !DILocation(line: 664, column: 44, scope: !1274)
!1282 = !DILocation(line: 665, column: 17, scope: !1274)
!1283 = !DILocation(line: 665, column: 23, scope: !1274)
!1284 = !DILocation(line: 665, column: 32, scope: !1274)
!1285 = !DILocation(line: 666, column: 17, scope: !1274)
!1286 = !DILocation(line: 669, column: 44, scope: !1287)
!1287 = distinct !DILexicalBlock(scope: !1275, file: !2, line: 668, column: 42)
!1288 = !DILocation(line: 669, column: 50, scope: !1287)
!1289 = !DILocation(line: 669, column: 64, scope: !1287)
!1290 = !DILocation(line: 669, column: 18, scope: !1287)
!1291 = !DILocation(line: 669, column: 24, scope: !1287)
!1292 = !DILocation(line: 669, column: 31, scope: !1287)
!1293 = !DILocation(line: 669, column: 42, scope: !1287)
!1294 = !DILocation(line: 670, column: 17, scope: !1287)
!1295 = !DILocation(line: 670, column: 23, scope: !1287)
!1296 = !DILocation(line: 670, column: 32, scope: !1287)
!1297 = !DILocation(line: 671, column: 17, scope: !1287)
!1298 = !DILocation(line: 674, column: 43, scope: !1299)
!1299 = distinct !DILexicalBlock(scope: !1275, file: !2, line: 673, column: 41)
!1300 = !DILocation(line: 674, column: 49, scope: !1299)
!1301 = !DILocation(line: 674, column: 63, scope: !1299)
!1302 = !DILocation(line: 674, column: 18, scope: !1299)
!1303 = !DILocation(line: 674, column: 24, scope: !1299)
!1304 = !DILocation(line: 674, column: 31, scope: !1299)
!1305 = !DILocation(line: 674, column: 41, scope: !1299)
!1306 = !DILocation(line: 675, column: 17, scope: !1299)
!1307 = !DILocation(line: 675, column: 23, scope: !1299)
!1308 = !DILocation(line: 675, column: 32, scope: !1299)
!1309 = !DILocation(line: 676, column: 17, scope: !1299)
!1310 = !DILocation(line: 679, column: 46, scope: !1311)
!1311 = distinct !DILexicalBlock(scope: !1275, file: !2, line: 678, column: 44)
!1312 = !DILocation(line: 679, column: 52, scope: !1311)
!1313 = !DILocation(line: 679, column: 66, scope: !1311)
!1314 = !DILocation(line: 679, column: 18, scope: !1311)
!1315 = !DILocation(line: 679, column: 24, scope: !1311)
!1316 = !DILocation(line: 679, column: 31, scope: !1311)
!1317 = !DILocation(line: 679, column: 44, scope: !1311)
!1318 = !DILocation(line: 680, column: 17, scope: !1311)
!1319 = !DILocation(line: 680, column: 23, scope: !1311)
!1320 = !DILocation(line: 680, column: 32, scope: !1311)
!1321 = !DILocation(line: 681, column: 17, scope: !1311)
!1322 = !DILocation(line: 684, column: 17, scope: !1275)
!1323 = !DILocation(line: 686, column: 5, scope: !1258)
!1324 = !DILocation(line: 657, column: 50, scope: !1253)
!1325 = !DILocation(line: 657, column: 5, scope: !1253)
!1326 = distinct !{!1326, !1256, !1327, !740}
!1327 = !DILocation(line: 686, column: 5, scope: !1249)
!1328 = !DILocation(line: 687, column: 1, scope: !1245)
!1329 = distinct !DISubprogram(name: "kgflags_print_errors", scope: !2, file: !2, line: 393, type: !1246, scopeLine: 393, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!1330 = !DILocalVariable(name: "i", scope: !1331, file: !2, line: 394, type: !38)
!1331 = distinct !DILexicalBlock(scope: !1329, file: !2, line: 394, column: 5)
!1332 = !DILocation(line: 394, column: 14, scope: !1331)
!1333 = !DILocation(line: 394, column: 10, scope: !1331)
!1334 = !DILocation(line: 394, column: 21, scope: !1335)
!1335 = distinct !DILexicalBlock(scope: !1331, file: !2, line: 394, column: 5)
!1336 = !DILocation(line: 394, column: 36, scope: !1335)
!1337 = !DILocation(line: 394, column: 23, scope: !1335)
!1338 = !DILocation(line: 394, column: 5, scope: !1331)
!1339 = !DILocalVariable(name: "err", scope: !1340, file: !2, line: 395, type: !1341)
!1340 = distinct !DILexicalBlock(scope: !1335, file: !2, line: 394, column: 55)
!1341 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !320, size: 64)
!1342 = !DILocation(line: 395, column: 27, scope: !1340)
!1343 = !DILocation(line: 395, column: 52, scope: !1340)
!1344 = !DILocation(line: 395, column: 34, scope: !1340)
!1345 = !DILocation(line: 396, column: 17, scope: !1340)
!1346 = !DILocation(line: 396, column: 22, scope: !1340)
!1347 = !DILocation(line: 396, column: 9, scope: !1340)
!1348 = !DILocation(line: 398, column: 25, scope: !1349)
!1349 = distinct !DILexicalBlock(scope: !1350, file: !2, line: 397, column: 52)
!1350 = distinct !DILexicalBlock(scope: !1340, file: !2, line: 396, column: 28)
!1351 = !DILocation(line: 398, column: 78, scope: !1349)
!1352 = !DILocation(line: 398, column: 92, scope: !1349)
!1353 = !DILocation(line: 398, column: 97, scope: !1349)
!1354 = !DILocation(line: 398, column: 17, scope: !1349)
!1355 = !DILocation(line: 399, column: 17, scope: !1349)
!1356 = !DILocation(line: 402, column: 25, scope: !1357)
!1357 = distinct !DILexicalBlock(scope: !1350, file: !2, line: 401, column: 51)
!1358 = !DILocation(line: 402, column: 73, scope: !1357)
!1359 = !DILocation(line: 402, column: 86, scope: !1357)
!1360 = !DILocation(line: 402, column: 91, scope: !1357)
!1361 = !DILocation(line: 402, column: 17, scope: !1357)
!1362 = !DILocation(line: 403, column: 17, scope: !1357)
!1363 = !DILocation(line: 406, column: 25, scope: !1364)
!1364 = distinct !DILexicalBlock(scope: !1350, file: !2, line: 405, column: 54)
!1365 = !DILocation(line: 406, column: 80, scope: !1364)
!1366 = !DILocation(line: 406, column: 93, scope: !1364)
!1367 = !DILocation(line: 406, column: 98, scope: !1364)
!1368 = !DILocation(line: 406, column: 17, scope: !1364)
!1369 = !DILocation(line: 407, column: 17, scope: !1364)
!1370 = !DILocation(line: 410, column: 25, scope: !1371)
!1371 = distinct !DILexicalBlock(scope: !1350, file: !2, line: 409, column: 50)
!1372 = !DILocation(line: 410, column: 105, scope: !1371)
!1373 = !DILocation(line: 410, column: 118, scope: !1371)
!1374 = !DILocation(line: 410, column: 123, scope: !1371)
!1375 = !DILocation(line: 410, column: 134, scope: !1371)
!1376 = !DILocation(line: 410, column: 139, scope: !1371)
!1377 = !DILocation(line: 410, column: 17, scope: !1371)
!1378 = !DILocation(line: 411, column: 17, scope: !1371)
!1379 = !DILocation(line: 414, column: 25, scope: !1380)
!1380 = distinct !DILexicalBlock(scope: !1350, file: !2, line: 413, column: 53)
!1381 = !DILocation(line: 414, column: 104, scope: !1380)
!1382 = !DILocation(line: 414, column: 117, scope: !1380)
!1383 = !DILocation(line: 414, column: 122, scope: !1380)
!1384 = !DILocation(line: 414, column: 133, scope: !1380)
!1385 = !DILocation(line: 414, column: 138, scope: !1380)
!1386 = !DILocation(line: 414, column: 17, scope: !1380)
!1387 = !DILocation(line: 415, column: 17, scope: !1380)
!1388 = !DILocation(line: 418, column: 25, scope: !1389)
!1389 = distinct !DILexicalBlock(scope: !1350, file: !2, line: 417, column: 53)
!1390 = !DILocation(line: 418, column: 17, scope: !1389)
!1391 = !DILocation(line: 419, column: 17, scope: !1389)
!1392 = !DILocation(line: 422, column: 25, scope: !1393)
!1393 = distinct !DILexicalBlock(scope: !1350, file: !2, line: 421, column: 61)
!1394 = !DILocation(line: 422, column: 17, scope: !1393)
!1395 = !DILocation(line: 423, column: 17, scope: !1393)
!1396 = !DILocation(line: 426, column: 25, scope: !1397)
!1397 = distinct !DILexicalBlock(scope: !1350, file: !2, line: 425, column: 58)
!1398 = !DILocation(line: 426, column: 83, scope: !1397)
!1399 = !DILocation(line: 426, column: 96, scope: !1397)
!1400 = !DILocation(line: 426, column: 101, scope: !1397)
!1401 = !DILocation(line: 426, column: 17, scope: !1397)
!1402 = !DILocation(line: 427, column: 17, scope: !1397)
!1403 = !DILocation(line: 430, column: 25, scope: !1404)
!1404 = distinct !DILexicalBlock(scope: !1350, file: !2, line: 429, column: 53)
!1405 = !DILocation(line: 430, column: 77, scope: !1404)
!1406 = !DILocation(line: 430, column: 90, scope: !1404)
!1407 = !DILocation(line: 430, column: 95, scope: !1404)
!1408 = !DILocation(line: 430, column: 17, scope: !1404)
!1409 = !DILocation(line: 431, column: 17, scope: !1404)
!1410 = !DILocation(line: 434, column: 25, scope: !1411)
!1411 = distinct !DILexicalBlock(scope: !1350, file: !2, line: 433, column: 48)
!1412 = !DILocation(line: 434, column: 103, scope: !1411)
!1413 = !DILocation(line: 434, column: 116, scope: !1411)
!1414 = !DILocation(line: 434, column: 121, scope: !1411)
!1415 = !DILocation(line: 434, column: 17, scope: !1411)
!1416 = !DILocation(line: 435, column: 17, scope: !1411)
!1417 = !DILocation(line: 438, column: 17, scope: !1350)
!1418 = !DILocation(line: 440, column: 5, scope: !1340)
!1419 = !DILocation(line: 394, column: 51, scope: !1335)
!1420 = !DILocation(line: 394, column: 5, scope: !1335)
!1421 = distinct !{!1421, !1338, !1422, !740}
!1422 = !DILocation(line: 440, column: 5, scope: !1331)
!1423 = !DILocation(line: 441, column: 1, scope: !1329)
!1424 = distinct !DISubprogram(name: "kgflags_print_usage", scope: !2, file: !2, line: 443, type: !1246, scopeLine: 443, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!1425 = !DILocation(line: 444, column: 20, scope: !1426)
!1426 = distinct !DILexicalBlock(scope: !1424, file: !2, line: 444, column: 9)
!1427 = !DILocation(line: 444, column: 39, scope: !1426)
!1428 = !DILocation(line: 445, column: 17, scope: !1429)
!1429 = distinct !DILexicalBlock(scope: !1426, file: !2, line: 444, column: 48)
!1430 = !DILocation(line: 445, column: 54, scope: !1429)
!1431 = !DILocation(line: 445, column: 43, scope: !1429)
!1432 = !DILocation(line: 445, column: 9, scope: !1429)
!1433 = !DILocation(line: 446, column: 5, scope: !1429)
!1434 = !DILocation(line: 447, column: 17, scope: !1435)
!1435 = distinct !DILexicalBlock(scope: !1426, file: !2, line: 446, column: 12)
!1436 = !DILocation(line: 447, column: 44, scope: !1435)
!1437 = !DILocation(line: 447, column: 9, scope: !1435)
!1438 = !DILocation(line: 450, column: 13, scope: !1424)
!1439 = !DILocation(line: 450, column: 5, scope: !1424)
!1440 = !DILocalVariable(name: "i", scope: !1441, file: !2, line: 451, type: !38)
!1441 = distinct !DILexicalBlock(scope: !1424, file: !2, line: 451, column: 5)
!1442 = !DILocation(line: 451, column: 14, scope: !1441)
!1443 = !DILocation(line: 451, column: 10, scope: !1441)
!1444 = !DILocation(line: 451, column: 21, scope: !1445)
!1445 = distinct !DILexicalBlock(scope: !1441, file: !2, line: 451, column: 5)
!1446 = !DILocation(line: 451, column: 36, scope: !1445)
!1447 = !DILocation(line: 451, column: 23, scope: !1445)
!1448 = !DILocation(line: 451, column: 5, scope: !1441)
!1449 = !DILocalVariable(name: "flag", scope: !1450, file: !2, line: 452, type: !713)
!1450 = distinct !DILexicalBlock(scope: !1445, file: !2, line: 451, column: 54)
!1451 = !DILocation(line: 452, column: 26, scope: !1450)
!1452 = !DILocation(line: 452, column: 51, scope: !1450)
!1453 = !DILocation(line: 452, column: 34, scope: !1450)
!1454 = !DILocation(line: 453, column: 17, scope: !1450)
!1455 = !DILocation(line: 453, column: 23, scope: !1450)
!1456 = !DILocation(line: 453, column: 9, scope: !1450)
!1457 = !DILocation(line: 455, column: 25, scope: !1458)
!1458 = distinct !DILexicalBlock(scope: !1450, file: !2, line: 453, column: 29)
!1459 = !DILocation(line: 455, column: 67, scope: !1458)
!1460 = !DILocation(line: 455, column: 80, scope: !1458)
!1461 = !DILocation(line: 455, column: 86, scope: !1458)
!1462 = !DILocation(line: 455, column: 92, scope: !1458)
!1463 = !DILocation(line: 455, column: 98, scope: !1458)
!1464 = !DILocation(line: 455, column: 17, scope: !1458)
!1465 = !DILocation(line: 456, column: 22, scope: !1466)
!1466 = distinct !DILexicalBlock(scope: !1458, file: !2, line: 456, column: 21)
!1467 = !DILocation(line: 456, column: 28, scope: !1466)
!1468 = !DILocation(line: 456, column: 21, scope: !1466)
!1469 = !DILocation(line: 457, column: 29, scope: !1470)
!1470 = distinct !DILexicalBlock(scope: !1466, file: !2, line: 456, column: 38)
!1471 = !DILocation(line: 457, column: 58, scope: !1470)
!1472 = !DILocation(line: 457, column: 64, scope: !1470)
!1473 = !DILocation(line: 457, column: 78, scope: !1470)
!1474 = !DILocation(line: 457, column: 21, scope: !1470)
!1475 = !DILocation(line: 458, column: 17, scope: !1470)
!1476 = !DILocation(line: 459, column: 17, scope: !1458)
!1477 = !DILocation(line: 461, column: 25, scope: !1478)
!1478 = distinct !DILexicalBlock(scope: !1458, file: !2, line: 460, column: 42)
!1479 = !DILocation(line: 461, column: 77, scope: !1478)
!1480 = !DILocation(line: 461, column: 90, scope: !1478)
!1481 = !DILocation(line: 461, column: 96, scope: !1478)
!1482 = !DILocation(line: 461, column: 113, scope: !1478)
!1483 = !DILocation(line: 461, column: 126, scope: !1478)
!1484 = !DILocation(line: 461, column: 132, scope: !1478)
!1485 = !DILocation(line: 462, column: 21, scope: !1478)
!1486 = !DILocation(line: 462, column: 27, scope: !1478)
!1487 = !DILocation(line: 461, column: 17, scope: !1478)
!1488 = !DILocation(line: 463, column: 22, scope: !1489)
!1489 = distinct !DILexicalBlock(scope: !1478, file: !2, line: 463, column: 21)
!1490 = !DILocation(line: 463, column: 28, scope: !1489)
!1491 = !DILocation(line: 463, column: 21, scope: !1489)
!1492 = !DILocation(line: 464, column: 29, scope: !1493)
!1493 = distinct !DILexicalBlock(scope: !1489, file: !2, line: 463, column: 38)
!1494 = !DILocation(line: 464, column: 58, scope: !1493)
!1495 = !DILocation(line: 464, column: 64, scope: !1493)
!1496 = !DILocation(line: 464, column: 78, scope: !1493)
!1497 = !DILocation(line: 464, column: 21, scope: !1493)
!1498 = !DILocation(line: 465, column: 17, scope: !1493)
!1499 = !DILocation(line: 466, column: 17, scope: !1478)
!1500 = !DILocation(line: 469, column: 25, scope: !1501)
!1501 = distinct !DILexicalBlock(scope: !1458, file: !2, line: 468, column: 41)
!1502 = !DILocation(line: 469, column: 68, scope: !1501)
!1503 = !DILocation(line: 469, column: 81, scope: !1501)
!1504 = !DILocation(line: 469, column: 87, scope: !1501)
!1505 = !DILocation(line: 469, column: 93, scope: !1501)
!1506 = !DILocation(line: 469, column: 99, scope: !1501)
!1507 = !DILocation(line: 469, column: 17, scope: !1501)
!1508 = !DILocation(line: 470, column: 22, scope: !1509)
!1509 = distinct !DILexicalBlock(scope: !1501, file: !2, line: 470, column: 21)
!1510 = !DILocation(line: 470, column: 28, scope: !1509)
!1511 = !DILocation(line: 470, column: 21, scope: !1509)
!1512 = !DILocation(line: 471, column: 29, scope: !1513)
!1513 = distinct !DILexicalBlock(scope: !1509, file: !2, line: 470, column: 38)
!1514 = !DILocation(line: 471, column: 58, scope: !1513)
!1515 = !DILocation(line: 471, column: 64, scope: !1513)
!1516 = !DILocation(line: 471, column: 78, scope: !1513)
!1517 = !DILocation(line: 471, column: 21, scope: !1513)
!1518 = !DILocation(line: 472, column: 17, scope: !1513)
!1519 = !DILocation(line: 473, column: 17, scope: !1501)
!1520 = !DILocation(line: 476, column: 25, scope: !1521)
!1521 = distinct !DILexicalBlock(scope: !1458, file: !2, line: 475, column: 44)
!1522 = !DILocation(line: 476, column: 66, scope: !1521)
!1523 = !DILocation(line: 476, column: 79, scope: !1521)
!1524 = !DILocation(line: 476, column: 85, scope: !1521)
!1525 = !DILocation(line: 476, column: 91, scope: !1521)
!1526 = !DILocation(line: 476, column: 97, scope: !1521)
!1527 = !DILocation(line: 476, column: 17, scope: !1521)
!1528 = !DILocation(line: 477, column: 22, scope: !1529)
!1529 = distinct !DILexicalBlock(scope: !1521, file: !2, line: 477, column: 21)
!1530 = !DILocation(line: 477, column: 28, scope: !1529)
!1531 = !DILocation(line: 477, column: 21, scope: !1529)
!1532 = !DILocation(line: 478, column: 29, scope: !1533)
!1533 = distinct !DILexicalBlock(scope: !1529, file: !2, line: 477, column: 38)
!1534 = !DILocation(line: 478, column: 61, scope: !1533)
!1535 = !DILocation(line: 478, column: 67, scope: !1533)
!1536 = !DILocation(line: 478, column: 81, scope: !1533)
!1537 = !DILocation(line: 478, column: 21, scope: !1533)
!1538 = !DILocation(line: 479, column: 17, scope: !1533)
!1539 = !DILocation(line: 480, column: 17, scope: !1521)
!1540 = !DILocation(line: 483, column: 25, scope: !1541)
!1541 = distinct !DILexicalBlock(scope: !1458, file: !2, line: 482, column: 50)
!1542 = !DILocation(line: 483, column: 77, scope: !1541)
!1543 = !DILocation(line: 483, column: 90, scope: !1541)
!1544 = !DILocation(line: 483, column: 96, scope: !1541)
!1545 = !DILocation(line: 483, column: 102, scope: !1541)
!1546 = !DILocation(line: 483, column: 108, scope: !1541)
!1547 = !DILocation(line: 483, column: 17, scope: !1541)
!1548 = !DILocation(line: 484, column: 17, scope: !1541)
!1549 = !DILocation(line: 487, column: 25, scope: !1550)
!1550 = distinct !DILexicalBlock(scope: !1458, file: !2, line: 486, column: 47)
!1551 = !DILocation(line: 487, column: 78, scope: !1550)
!1552 = !DILocation(line: 487, column: 91, scope: !1550)
!1553 = !DILocation(line: 487, column: 97, scope: !1550)
!1554 = !DILocation(line: 487, column: 103, scope: !1550)
!1555 = !DILocation(line: 487, column: 109, scope: !1550)
!1556 = !DILocation(line: 487, column: 17, scope: !1550)
!1557 = !DILocation(line: 488, column: 17, scope: !1550)
!1558 = !DILocation(line: 491, column: 25, scope: !1559)
!1559 = distinct !DILexicalBlock(scope: !1458, file: !2, line: 490, column: 50)
!1560 = !DILocation(line: 491, column: 76, scope: !1559)
!1561 = !DILocation(line: 491, column: 89, scope: !1559)
!1562 = !DILocation(line: 491, column: 95, scope: !1559)
!1563 = !DILocation(line: 491, column: 101, scope: !1559)
!1564 = !DILocation(line: 491, column: 107, scope: !1559)
!1565 = !DILocation(line: 491, column: 17, scope: !1559)
!1566 = !DILocation(line: 492, column: 17, scope: !1559)
!1567 = !DILocation(line: 495, column: 17, scope: !1458)
!1568 = !DILocation(line: 497, column: 13, scope: !1569)
!1569 = distinct !DILexicalBlock(scope: !1450, file: !2, line: 497, column: 13)
!1570 = !DILocation(line: 497, column: 19, scope: !1569)
!1571 = !DILocation(line: 498, column: 21, scope: !1572)
!1572 = distinct !DILexicalBlock(scope: !1569, file: !2, line: 497, column: 32)
!1573 = !DILocation(line: 498, column: 41, scope: !1572)
!1574 = !DILocation(line: 498, column: 47, scope: !1572)
!1575 = !DILocation(line: 498, column: 13, scope: !1572)
!1576 = !DILocation(line: 499, column: 9, scope: !1572)
!1577 = !DILocation(line: 500, column: 17, scope: !1450)
!1578 = !DILocation(line: 500, column: 9, scope: !1450)
!1579 = !DILocation(line: 501, column: 5, scope: !1450)
!1580 = !DILocation(line: 451, column: 50, scope: !1445)
!1581 = !DILocation(line: 451, column: 5, scope: !1445)
!1582 = distinct !{!1582, !1448, !1583, !740}
!1583 = !DILocation(line: 501, column: 5, scope: !1441)
!1584 = !DILocation(line: 502, column: 1, scope: !1424)
!1585 = distinct !DISubprogram(name: "kgflags_set_custom_description", scope: !2, file: !2, line: 504, type: !675, scopeLine: 504, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!1586 = !DILocalVariable(name: "description", arg: 1, scope: !1585, file: !2, line: 504, type: !259)
!1587 = !DILocation(line: 504, column: 49, scope: !1585)
!1588 = !DILocation(line: 505, column: 37, scope: !1585)
!1589 = !DILocation(line: 505, column: 35, scope: !1585)
!1590 = !DILocation(line: 506, column: 1, scope: !1585)
!1591 = distinct !DISubprogram(name: "kgflags_string_array_get_count", scope: !2, file: !2, line: 508, type: !1592, scopeLine: 508, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!1592 = !DISubroutineType(types: !1593)
!1593 = !{!38, !1594}
!1594 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1595, size: 64)
!1595 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !284)
!1596 = !DILocalVariable(name: "arr", arg: 1, scope: !1591, file: !2, line: 508, type: !1594)
!1597 = !DILocation(line: 508, column: 66, scope: !1591)
!1598 = !DILocation(line: 509, column: 12, scope: !1591)
!1599 = !DILocation(line: 509, column: 17, scope: !1591)
!1600 = !DILocation(line: 509, column: 5, scope: !1591)
!1601 = distinct !DISubprogram(name: "kgflags_string_array_get_item", scope: !2, file: !2, line: 512, type: !1602, scopeLine: 512, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!1602 = !DISubroutineType(types: !1603)
!1603 = !{!259, !1594, !38}
!1604 = !DILocalVariable(name: "arr", arg: 1, scope: !1601, file: !2, line: 512, type: !1594)
!1605 = !DILocation(line: 512, column: 73, scope: !1601)
!1606 = !DILocalVariable(name: "at", arg: 2, scope: !1601, file: !2, line: 512, type: !38)
!1607 = !DILocation(line: 512, column: 82, scope: !1601)
!1608 = !DILocation(line: 513, column: 9, scope: !1609)
!1609 = distinct !DILexicalBlock(scope: !1601, file: !2, line: 513, column: 9)
!1610 = !DILocation(line: 513, column: 12, scope: !1609)
!1611 = !DILocation(line: 513, column: 16, scope: !1609)
!1612 = !DILocation(line: 513, column: 19, scope: !1609)
!1613 = !DILocation(line: 513, column: 25, scope: !1609)
!1614 = !DILocation(line: 513, column: 30, scope: !1609)
!1615 = !DILocation(line: 513, column: 22, scope: !1609)
!1616 = !DILocation(line: 514, column: 9, scope: !1617)
!1617 = distinct !DILexicalBlock(scope: !1609, file: !2, line: 513, column: 38)
!1618 = !DILocation(line: 516, column: 12, scope: !1601)
!1619 = !DILocation(line: 516, column: 17, scope: !1601)
!1620 = !DILocation(line: 516, column: 24, scope: !1601)
!1621 = !DILocation(line: 516, column: 5, scope: !1601)
!1622 = !DILocation(line: 517, column: 1, scope: !1601)
!1623 = distinct !DISubprogram(name: "kgflags_int_array_get_count", scope: !2, file: !2, line: 519, type: !1624, scopeLine: 519, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!1624 = !DISubroutineType(types: !1625)
!1625 = !{!38, !1626}
!1626 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1627, size: 64)
!1627 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !293)
!1628 = !DILocalVariable(name: "arr", arg: 1, scope: !1623, file: !2, line: 519, type: !1626)
!1629 = !DILocation(line: 519, column: 60, scope: !1623)
!1630 = !DILocation(line: 520, column: 12, scope: !1623)
!1631 = !DILocation(line: 520, column: 17, scope: !1623)
!1632 = !DILocation(line: 520, column: 5, scope: !1623)
!1633 = distinct !DISubprogram(name: "kgflags_int_array_get_item", scope: !2, file: !2, line: 523, type: !1634, scopeLine: 523, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!1634 = !DISubroutineType(types: !1635)
!1635 = !{!38, !1626, !38}
!1636 = !DILocalVariable(name: "arr", arg: 1, scope: !1633, file: !2, line: 523, type: !1626)
!1637 = !DILocation(line: 523, column: 59, scope: !1633)
!1638 = !DILocalVariable(name: "at", arg: 2, scope: !1633, file: !2, line: 523, type: !38)
!1639 = !DILocation(line: 523, column: 68, scope: !1633)
!1640 = !DILocation(line: 524, column: 9, scope: !1641)
!1641 = distinct !DILexicalBlock(scope: !1633, file: !2, line: 524, column: 9)
!1642 = !DILocation(line: 524, column: 12, scope: !1641)
!1643 = !DILocation(line: 524, column: 16, scope: !1641)
!1644 = !DILocation(line: 524, column: 19, scope: !1641)
!1645 = !DILocation(line: 524, column: 25, scope: !1641)
!1646 = !DILocation(line: 524, column: 30, scope: !1641)
!1647 = !DILocation(line: 524, column: 22, scope: !1641)
!1648 = !DILocation(line: 525, column: 9, scope: !1649)
!1649 = distinct !DILexicalBlock(scope: !1641, file: !2, line: 524, column: 38)
!1650 = !DILocalVariable(name: "str", scope: !1633, file: !2, line: 527, type: !259)
!1651 = !DILocation(line: 527, column: 17, scope: !1633)
!1652 = !DILocation(line: 527, column: 23, scope: !1633)
!1653 = !DILocation(line: 527, column: 28, scope: !1633)
!1654 = !DILocation(line: 527, column: 35, scope: !1633)
!1655 = !DILocalVariable(name: "ok", scope: !1633, file: !2, line: 528, type: !267)
!1656 = !DILocation(line: 528, column: 10, scope: !1633)
!1657 = !DILocalVariable(name: "res", scope: !1633, file: !2, line: 529, type: !38)
!1658 = !DILocation(line: 529, column: 9, scope: !1633)
!1659 = !DILocation(line: 529, column: 34, scope: !1633)
!1660 = !DILocation(line: 529, column: 15, scope: !1633)
!1661 = !DILocation(line: 530, column: 10, scope: !1662)
!1662 = distinct !DILexicalBlock(scope: !1633, file: !2, line: 530, column: 9)
!1663 = !DILocation(line: 530, column: 9, scope: !1662)
!1664 = !DILocation(line: 531, column: 9, scope: !1665)
!1665 = distinct !DILexicalBlock(scope: !1662, file: !2, line: 530, column: 14)
!1666 = !DILocation(line: 533, column: 12, scope: !1633)
!1667 = !DILocation(line: 533, column: 5, scope: !1633)
!1668 = !DILocation(line: 534, column: 1, scope: !1633)
!1669 = distinct !DISubprogram(name: "_kgflags_parse_int", scope: !2, file: !2, line: 617, type: !1670, scopeLine: 617, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !9, retainedNodes: !343)
!1670 = !DISubroutineType(types: !1671)
!1671 = !{!38, !259, !277}
!1672 = !DILocalVariable(name: "str", arg: 1, scope: !1669, file: !2, line: 617, type: !259)
!1673 = !DILocation(line: 617, column: 43, scope: !1669)
!1674 = !DILocalVariable(name: "out_ok", arg: 2, scope: !1669, file: !2, line: 617, type: !277)
!1675 = !DILocation(line: 617, column: 54, scope: !1669)
!1676 = !DILocation(line: 618, column: 6, scope: !1669)
!1677 = !DILocation(line: 618, column: 13, scope: !1669)
!1678 = !DILocalVariable(name: "end", scope: !1669, file: !2, line: 619, type: !289)
!1679 = !DILocation(line: 619, column: 11, scope: !1669)
!1680 = !DILocalVariable(name: "res_l", scope: !1669, file: !2, line: 620, type: !1681)
!1681 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!1682 = !DILocation(line: 620, column: 10, scope: !1669)
!1683 = !DILocation(line: 620, column: 25, scope: !1669)
!1684 = !DILocation(line: 620, column: 18, scope: !1669)
!1685 = !DILocation(line: 621, column: 9, scope: !1686)
!1686 = distinct !DILexicalBlock(scope: !1669, file: !2, line: 621, column: 9)
!1687 = !DILocation(line: 621, column: 16, scope: !1686)
!1688 = !DILocation(line: 621, column: 13, scope: !1686)
!1689 = !DILocation(line: 621, column: 20, scope: !1686)
!1690 = !DILocation(line: 621, column: 24, scope: !1686)
!1691 = !DILocation(line: 621, column: 23, scope: !1686)
!1692 = !DILocation(line: 621, column: 28, scope: !1686)
!1693 = !DILocation(line: 621, column: 36, scope: !1686)
!1694 = !DILocation(line: 621, column: 39, scope: !1686)
!1695 = !DILocation(line: 621, column: 45, scope: !1686)
!1696 = !DILocation(line: 621, column: 55, scope: !1686)
!1697 = !DILocation(line: 621, column: 58, scope: !1686)
!1698 = !DILocation(line: 621, column: 64, scope: !1686)
!1699 = !DILocation(line: 622, column: 5, scope: !1686)
!1700 = !DILocation(line: 622, column: 10, scope: !1686)
!1701 = !DILocation(line: 622, column: 16, scope: !1686)
!1702 = !DILocation(line: 622, column: 28, scope: !1686)
!1703 = !DILocation(line: 622, column: 31, scope: !1686)
!1704 = !DILocation(line: 622, column: 37, scope: !1686)
!1705 = !DILocation(line: 622, column: 50, scope: !1686)
!1706 = !DILocation(line: 622, column: 63, scope: !1686)
!1707 = !DILocation(line: 622, column: 60, scope: !1686)
!1708 = !DILocation(line: 623, column: 10, scope: !1709)
!1709 = distinct !DILexicalBlock(scope: !1686, file: !2, line: 622, column: 71)
!1710 = !DILocation(line: 623, column: 17, scope: !1709)
!1711 = !DILocation(line: 624, column: 9, scope: !1709)
!1712 = !DILocation(line: 626, column: 6, scope: !1669)
!1713 = !DILocation(line: 626, column: 13, scope: !1669)
!1714 = !DILocation(line: 627, column: 17, scope: !1669)
!1715 = !DILocation(line: 627, column: 12, scope: !1669)
!1716 = !DILocation(line: 627, column: 5, scope: !1669)
!1717 = !DILocation(line: 628, column: 1, scope: !1669)
!1718 = distinct !DISubprogram(name: "kgflags_double_array_get_count", scope: !2, file: !2, line: 536, type: !1719, scopeLine: 536, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!1719 = !DISubroutineType(types: !1720)
!1720 = !{!38, !1721}
!1721 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !1722, size: 64)
!1722 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !300)
!1723 = !DILocalVariable(name: "arr", arg: 1, scope: !1718, file: !2, line: 536, type: !1721)
!1724 = !DILocation(line: 536, column: 66, scope: !1718)
!1725 = !DILocation(line: 537, column: 12, scope: !1718)
!1726 = !DILocation(line: 537, column: 17, scope: !1718)
!1727 = !DILocation(line: 537, column: 5, scope: !1718)
!1728 = distinct !DISubprogram(name: "kgflags_double_array_get_item", scope: !2, file: !2, line: 540, type: !1729, scopeLine: 540, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!1729 = !DISubroutineType(types: !1730)
!1730 = !{!270, !1721, !38}
!1731 = !DILocalVariable(name: "arr", arg: 1, scope: !1728, file: !2, line: 540, type: !1721)
!1732 = !DILocation(line: 540, column: 68, scope: !1728)
!1733 = !DILocalVariable(name: "at", arg: 2, scope: !1728, file: !2, line: 540, type: !38)
!1734 = !DILocation(line: 540, column: 77, scope: !1728)
!1735 = !DILocation(line: 541, column: 9, scope: !1736)
!1736 = distinct !DILexicalBlock(scope: !1728, file: !2, line: 541, column: 9)
!1737 = !DILocation(line: 541, column: 12, scope: !1736)
!1738 = !DILocation(line: 541, column: 16, scope: !1736)
!1739 = !DILocation(line: 541, column: 19, scope: !1736)
!1740 = !DILocation(line: 541, column: 25, scope: !1736)
!1741 = !DILocation(line: 541, column: 30, scope: !1736)
!1742 = !DILocation(line: 541, column: 22, scope: !1736)
!1743 = !DILocation(line: 542, column: 9, scope: !1744)
!1744 = distinct !DILexicalBlock(scope: !1736, file: !2, line: 541, column: 38)
!1745 = !DILocalVariable(name: "str", scope: !1728, file: !2, line: 544, type: !259)
!1746 = !DILocation(line: 544, column: 17, scope: !1728)
!1747 = !DILocation(line: 544, column: 23, scope: !1728)
!1748 = !DILocation(line: 544, column: 28, scope: !1728)
!1749 = !DILocation(line: 544, column: 35, scope: !1728)
!1750 = !DILocalVariable(name: "ok", scope: !1728, file: !2, line: 545, type: !267)
!1751 = !DILocation(line: 545, column: 10, scope: !1728)
!1752 = !DILocalVariable(name: "res", scope: !1728, file: !2, line: 546, type: !270)
!1753 = !DILocation(line: 546, column: 12, scope: !1728)
!1754 = !DILocation(line: 546, column: 40, scope: !1728)
!1755 = !DILocation(line: 546, column: 18, scope: !1728)
!1756 = !DILocation(line: 547, column: 10, scope: !1757)
!1757 = distinct !DILexicalBlock(scope: !1728, file: !2, line: 547, column: 9)
!1758 = !DILocation(line: 547, column: 9, scope: !1757)
!1759 = !DILocation(line: 548, column: 9, scope: !1760)
!1760 = distinct !DILexicalBlock(scope: !1757, file: !2, line: 547, column: 14)
!1761 = !DILocation(line: 550, column: 12, scope: !1728)
!1762 = !DILocation(line: 550, column: 5, scope: !1728)
!1763 = !DILocation(line: 551, column: 1, scope: !1728)
!1764 = distinct !DISubprogram(name: "_kgflags_parse_double", scope: !2, file: !2, line: 630, type: !1765, scopeLine: 630, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !9, retainedNodes: !343)
!1765 = !DISubroutineType(types: !1766)
!1766 = !{!270, !259, !277}
!1767 = !DILocalVariable(name: "str", arg: 1, scope: !1764, file: !2, line: 630, type: !259)
!1768 = !DILocation(line: 630, column: 49, scope: !1764)
!1769 = !DILocalVariable(name: "out_ok", arg: 2, scope: !1764, file: !2, line: 630, type: !277)
!1770 = !DILocation(line: 630, column: 60, scope: !1764)
!1771 = !DILocation(line: 631, column: 6, scope: !1764)
!1772 = !DILocation(line: 631, column: 13, scope: !1764)
!1773 = !DILocalVariable(name: "end", scope: !1764, file: !2, line: 632, type: !289)
!1774 = !DILocation(line: 632, column: 11, scope: !1764)
!1775 = !DILocalVariable(name: "res", scope: !1764, file: !2, line: 633, type: !270)
!1776 = !DILocation(line: 633, column: 12, scope: !1764)
!1777 = !DILocation(line: 633, column: 25, scope: !1764)
!1778 = !DILocation(line: 633, column: 18, scope: !1764)
!1779 = !DILocation(line: 634, column: 9, scope: !1780)
!1780 = distinct !DILexicalBlock(scope: !1764, file: !2, line: 634, column: 9)
!1781 = !DILocation(line: 634, column: 16, scope: !1780)
!1782 = !DILocation(line: 634, column: 13, scope: !1780)
!1783 = !DILocation(line: 634, column: 20, scope: !1780)
!1784 = !DILocation(line: 634, column: 24, scope: !1780)
!1785 = !DILocation(line: 634, column: 23, scope: !1780)
!1786 = !DILocation(line: 634, column: 28, scope: !1780)
!1787 = !DILocation(line: 635, column: 5, scope: !1780)
!1788 = !DILocation(line: 635, column: 10, scope: !1780)
!1789 = !DILocation(line: 635, column: 14, scope: !1780)
!1790 = !DILocation(line: 635, column: 27, scope: !1780)
!1791 = !DILocation(line: 635, column: 30, scope: !1780)
!1792 = !DILocation(line: 635, column: 34, scope: !1780)
!1793 = !DILocation(line: 635, column: 48, scope: !1780)
!1794 = !DILocation(line: 635, column: 61, scope: !1780)
!1795 = !DILocation(line: 635, column: 58, scope: !1780)
!1796 = !DILocation(line: 636, column: 10, scope: !1797)
!1797 = distinct !DILexicalBlock(scope: !1780, file: !2, line: 635, column: 69)
!1798 = !DILocation(line: 636, column: 17, scope: !1797)
!1799 = !DILocation(line: 637, column: 9, scope: !1797)
!1800 = !DILocation(line: 639, column: 6, scope: !1764)
!1801 = !DILocation(line: 639, column: 13, scope: !1764)
!1802 = !DILocation(line: 640, column: 12, scope: !1764)
!1803 = !DILocation(line: 640, column: 5, scope: !1764)
!1804 = !DILocation(line: 641, column: 1, scope: !1764)
!1805 = distinct !DISubprogram(name: "kgflags_get_non_flag_args_count", scope: !2, file: !2, line: 553, type: !1806, scopeLine: 553, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9)
!1806 = !DISubroutineType(types: !1807)
!1807 = !{!38}
!1808 = !DILocation(line: 554, column: 23, scope: !1805)
!1809 = !DILocation(line: 554, column: 5, scope: !1805)
!1810 = distinct !DISubprogram(name: "kgflags_get_non_flag_arg", scope: !2, file: !2, line: 557, type: !1811, scopeLine: 557, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!1811 = !DISubroutineType(types: !1812)
!1812 = !{!259, !38}
!1813 = !DILocalVariable(name: "at", arg: 1, scope: !1810, file: !2, line: 557, type: !38)
!1814 = !DILocation(line: 557, column: 42, scope: !1810)
!1815 = !DILocation(line: 558, column: 9, scope: !1816)
!1816 = distinct !DILexicalBlock(scope: !1810, file: !2, line: 558, column: 9)
!1817 = !DILocation(line: 558, column: 12, scope: !1816)
!1818 = !DILocation(line: 558, column: 16, scope: !1816)
!1819 = !DILocation(line: 558, column: 19, scope: !1816)
!1820 = !DILocation(line: 558, column: 36, scope: !1816)
!1821 = !DILocation(line: 558, column: 22, scope: !1816)
!1822 = !DILocation(line: 559, column: 9, scope: !1823)
!1823 = distinct !DILexicalBlock(scope: !1816, file: !2, line: 558, column: 52)
!1824 = !DILocation(line: 561, column: 37, scope: !1810)
!1825 = !DILocation(line: 561, column: 12, scope: !1810)
!1826 = !DILocation(line: 561, column: 5, scope: !1810)
!1827 = !DILocation(line: 562, column: 1, scope: !1810)
!1828 = distinct !DISubprogram(name: "main", scope: !2, file: !2, line: 845, type: !1829, scopeLine: 845, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !9, retainedNodes: !343)
!1829 = !DISubroutineType(types: !1830)
!1830 = !{!38, !38, !288}
!1831 = !DILocalVariable(name: "argc", arg: 1, scope: !1828, file: !2, line: 845, type: !38)
!1832 = !DILocation(line: 845, column: 14, scope: !1828)
!1833 = !DILocalVariable(name: "argv", arg: 2, scope: !1828, file: !2, line: 845, type: !288)
!1834 = !DILocation(line: 845, column: 27, scope: !1828)
!1835 = !DILocalVariable(name: "string_val", scope: !1828, file: !2, line: 846, type: !259)
!1836 = !DILocation(line: 846, column: 17, scope: !1828)
!1837 = !DILocation(line: 847, column: 5, scope: !1828)
!1838 = !DILocalVariable(name: "bool_val", scope: !1828, file: !2, line: 849, type: !267)
!1839 = !DILocation(line: 849, column: 10, scope: !1828)
!1840 = !DILocation(line: 850, column: 5, scope: !1828)
!1841 = !DILocalVariable(name: "int_val", scope: !1828, file: !2, line: 852, type: !38)
!1842 = !DILocation(line: 852, column: 9, scope: !1828)
!1843 = !DILocation(line: 853, column: 5, scope: !1828)
!1844 = !DILocalVariable(name: "double_val", scope: !1828, file: !2, line: 855, type: !270)
!1845 = !DILocation(line: 855, column: 12, scope: !1828)
!1846 = !DILocation(line: 856, column: 5, scope: !1828)
!1847 = !DILocalVariable(name: "string_arr", scope: !1828, file: !2, line: 858, type: !284)
!1848 = !DILocation(line: 858, column: 28, scope: !1828)
!1849 = !DILocation(line: 859, column: 5, scope: !1828)
!1850 = !DILocalVariable(name: "int_arr", scope: !1828, file: !2, line: 861, type: !293)
!1851 = !DILocation(line: 861, column: 25, scope: !1828)
!1852 = !DILocation(line: 862, column: 5, scope: !1828)
!1853 = !DILocalVariable(name: "double_arr", scope: !1828, file: !2, line: 864, type: !300)
!1854 = !DILocation(line: 864, column: 28, scope: !1828)
!1855 = !DILocation(line: 865, column: 5, scope: !1828)
!1856 = !DILocation(line: 867, column: 5, scope: !1828)
!1857 = !DILocation(line: 868, column: 5, scope: !1828)
!1858 = !DILocation(line: 869, column: 24, scope: !1859)
!1859 = distinct !DILexicalBlock(scope: !1828, file: !2, line: 869, column: 9)
!1860 = !DILocation(line: 869, column: 30, scope: !1859)
!1861 = !DILocation(line: 869, column: 10, scope: !1859)
!1862 = !DILocation(line: 869, column: 9, scope: !1859)
!1863 = !DILocation(line: 870, column: 9, scope: !1864)
!1864 = distinct !DILexicalBlock(scope: !1859, file: !2, line: 869, column: 37)
!1865 = !DILocation(line: 871, column: 9, scope: !1864)
!1866 = !DILocation(line: 872, column: 9, scope: !1864)
!1867 = !DILocation(line: 875, column: 32, scope: !1828)
!1868 = !DILocation(line: 875, column: 5, scope: !1828)
!1869 = !DILocation(line: 876, column: 30, scope: !1828)
!1870 = !DILocation(line: 876, column: 5, scope: !1828)
!1871 = !DILocation(line: 877, column: 29, scope: !1828)
!1872 = !DILocation(line: 877, column: 5, scope: !1828)
!1873 = !DILocation(line: 878, column: 32, scope: !1828)
!1874 = !DILocation(line: 878, column: 5, scope: !1828)
!1875 = !DILocation(line: 880, column: 38, scope: !1828)
!1876 = !DILocation(line: 880, column: 5, scope: !1828)
!1877 = !DILocalVariable(name: "i", scope: !1878, file: !2, line: 881, type: !38)
!1878 = distinct !DILexicalBlock(scope: !1828, file: !2, line: 881, column: 5)
!1879 = !DILocation(line: 881, column: 14, scope: !1878)
!1880 = !DILocation(line: 881, column: 10, scope: !1878)
!1881 = !DILocation(line: 881, column: 21, scope: !1882)
!1882 = distinct !DILexicalBlock(scope: !1878, file: !2, line: 881, column: 5)
!1883 = !DILocation(line: 881, column: 25, scope: !1882)
!1884 = !DILocation(line: 881, column: 23, scope: !1882)
!1885 = !DILocation(line: 881, column: 5, scope: !1878)
!1886 = !DILocation(line: 882, column: 41, scope: !1887)
!1887 = distinct !DILexicalBlock(scope: !1882, file: !2, line: 881, column: 75)
!1888 = !DILocation(line: 882, column: 87, scope: !1887)
!1889 = !DILocation(line: 882, column: 44, scope: !1887)
!1890 = !DILocation(line: 882, column: 9, scope: !1887)
!1891 = !DILocation(line: 883, column: 5, scope: !1887)
!1892 = !DILocation(line: 881, column: 71, scope: !1882)
!1893 = !DILocation(line: 881, column: 5, scope: !1882)
!1894 = distinct !{!1894, !1885, !1895, !740}
!1895 = !DILocation(line: 883, column: 5, scope: !1878)
!1896 = !DILocation(line: 885, column: 35, scope: !1828)
!1897 = !DILocation(line: 885, column: 5, scope: !1828)
!1898 = !DILocalVariable(name: "i", scope: !1899, file: !2, line: 886, type: !38)
!1899 = distinct !DILexicalBlock(scope: !1828, file: !2, line: 886, column: 5)
!1900 = !DILocation(line: 886, column: 14, scope: !1899)
!1901 = !DILocation(line: 886, column: 10, scope: !1899)
!1902 = !DILocation(line: 886, column: 21, scope: !1903)
!1903 = distinct !DILexicalBlock(scope: !1899, file: !2, line: 886, column: 5)
!1904 = !DILocation(line: 886, column: 25, scope: !1903)
!1905 = !DILocation(line: 886, column: 23, scope: !1903)
!1906 = !DILocation(line: 886, column: 5, scope: !1899)
!1907 = !DILocation(line: 887, column: 38, scope: !1908)
!1908 = distinct !DILexicalBlock(scope: !1903, file: !2, line: 886, column: 69)
!1909 = !DILocation(line: 887, column: 78, scope: !1908)
!1910 = !DILocation(line: 887, column: 41, scope: !1908)
!1911 = !DILocation(line: 887, column: 9, scope: !1908)
!1912 = !DILocation(line: 888, column: 5, scope: !1908)
!1913 = !DILocation(line: 886, column: 65, scope: !1903)
!1914 = !DILocation(line: 886, column: 5, scope: !1903)
!1915 = distinct !{!1915, !1906, !1916, !740}
!1916 = !DILocation(line: 888, column: 5, scope: !1899)
!1917 = !DILocation(line: 890, column: 38, scope: !1828)
!1918 = !DILocation(line: 890, column: 5, scope: !1828)
!1919 = !DILocalVariable(name: "i", scope: !1920, file: !2, line: 891, type: !38)
!1920 = distinct !DILexicalBlock(scope: !1828, file: !2, line: 891, column: 5)
!1921 = !DILocation(line: 891, column: 14, scope: !1920)
!1922 = !DILocation(line: 891, column: 10, scope: !1920)
!1923 = !DILocation(line: 891, column: 21, scope: !1924)
!1924 = distinct !DILexicalBlock(scope: !1920, file: !2, line: 891, column: 5)
!1925 = !DILocation(line: 891, column: 25, scope: !1924)
!1926 = !DILocation(line: 891, column: 23, scope: !1924)
!1927 = !DILocation(line: 891, column: 5, scope: !1920)
!1928 = !DILocation(line: 892, column: 44, scope: !1929)
!1929 = distinct !DILexicalBlock(scope: !1924, file: !2, line: 891, column: 75)
!1930 = !DILocation(line: 892, column: 90, scope: !1929)
!1931 = !DILocation(line: 892, column: 47, scope: !1929)
!1932 = !DILocation(line: 892, column: 9, scope: !1929)
!1933 = !DILocation(line: 893, column: 5, scope: !1929)
!1934 = !DILocation(line: 891, column: 71, scope: !1924)
!1935 = !DILocation(line: 891, column: 5, scope: !1924)
!1936 = distinct !{!1936, !1927, !1937, !740}
!1937 = !DILocation(line: 893, column: 5, scope: !1920)
!1938 = !DILocalVariable(name: "non_flag_count", scope: !1828, file: !2, line: 895, type: !38)
!1939 = !DILocation(line: 895, column: 9, scope: !1828)
!1940 = !DILocation(line: 895, column: 26, scope: !1828)
!1941 = !DILocalVariable(name: "i", scope: !1942, file: !2, line: 896, type: !38)
!1942 = distinct !DILexicalBlock(scope: !1828, file: !2, line: 896, column: 5)
!1943 = !DILocation(line: 896, column: 14, scope: !1942)
!1944 = !DILocation(line: 896, column: 10, scope: !1942)
!1945 = !DILocation(line: 896, column: 21, scope: !1946)
!1946 = distinct !DILexicalBlock(scope: !1942, file: !2, line: 896, column: 5)
!1947 = !DILocation(line: 896, column: 25, scope: !1946)
!1948 = !DILocation(line: 896, column: 23, scope: !1946)
!1949 = !DILocation(line: 896, column: 5, scope: !1942)
!1950 = !DILocation(line: 897, column: 50, scope: !1951)
!1951 = distinct !DILexicalBlock(scope: !1946, file: !2, line: 896, column: 46)
!1952 = !DILocation(line: 897, column: 78, scope: !1951)
!1953 = !DILocation(line: 897, column: 53, scope: !1951)
!1954 = !DILocation(line: 897, column: 9, scope: !1951)
!1955 = !DILocation(line: 898, column: 5, scope: !1951)
!1956 = !DILocation(line: 896, column: 42, scope: !1946)
!1957 = !DILocation(line: 896, column: 5, scope: !1946)
!1958 = distinct !{!1958, !1949, !1959, !740}
!1959 = !DILocation(line: 898, column: 5, scope: !1942)
!1960 = !DILocation(line: 900, column: 5, scope: !1828)
!1961 = !DILocation(line: 901, column: 1, scope: !1828)
!1962 = distinct !DISubprogram(name: "_kgflags_peek_arg", scope: !2, file: !2, line: 708, type: !799, scopeLine: 708, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !9)
!1963 = !DILocation(line: 709, column: 20, scope: !1964)
!1964 = distinct !DILexicalBlock(scope: !1962, file: !2, line: 709, column: 9)
!1965 = !DILocation(line: 709, column: 45, scope: !1964)
!1966 = !DILocation(line: 709, column: 31, scope: !1964)
!1967 = !DILocation(line: 710, column: 9, scope: !1968)
!1968 = distinct !DILexicalBlock(scope: !1964, file: !2, line: 709, column: 51)
!1969 = !DILocation(line: 712, column: 23, scope: !1962)
!1970 = !DILocation(line: 712, column: 39, scope: !1962)
!1971 = !DILocation(line: 712, column: 12, scope: !1962)
!1972 = !DILocation(line: 712, column: 5, scope: !1962)
!1973 = !DILocation(line: 713, column: 1, scope: !1962)
