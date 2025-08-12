; ModuleID = 'dynarr.c'
source_filename = "dynarr.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.dynarr = type { i32, i32, i32, i32 }

@.str = private unnamed_addr constant [15 x i8] c"darr[%d] = %d\0A\00", align 1, !dbg !0
@.str.1 = private unnamed_addr constant [56 x i8] c"(((i) >= 0)&&((i) < ((struct dynarr*)(darr))[-1].size))\00", align 1, !dbg !7
@.str.2 = private unnamed_addr constant [9 x i8] c"dynarr.c\00", align 1, !dbg !12
@__PRETTY_FUNCTION__.main = private unnamed_addr constant [11 x i8] c"int main()\00", align 1, !dbg !17

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @main() #0 !dbg !51 {
  %1 = alloca i32, align 4
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  store i32 0, ptr %1, align 4
    #dbg_declare(ptr %2, !55, !DIExpression(), !56)
  %4 = call ptr @dynarrNew(i32 noundef 4), !dbg !57
  store ptr %4, ptr %2, align 8, !dbg !56
  %5 = call i32 @dynarrGrow(ptr noundef %2), !dbg !58
  %6 = icmp ne i32 %5, 0, !dbg !58
  br i1 %6, label %7, label %8, !dbg !58

7:                                                ; preds = %0
  br label %26, !dbg !58

8:                                                ; preds = %0
  %9 = load ptr, ptr %2, align 8, !dbg !58
  %10 = load ptr, ptr %2, align 8, !dbg !58
  %11 = getelementptr inbounds %struct.dynarr, ptr %10, i64 -1, !dbg !58
  %12 = getelementptr inbounds nuw %struct.dynarr, ptr %11, i32 0, i32 2, !dbg !58
  %13 = load i32, ptr %12, align 4, !dbg !58
  %14 = load ptr, ptr %2, align 8, !dbg !58
  %15 = getelementptr inbounds %struct.dynarr, ptr %14, i64 -1, !dbg !58
  %16 = getelementptr inbounds nuw %struct.dynarr, ptr %15, i32 0, i32 3, !dbg !58
  %17 = load i32, ptr %16, align 4, !dbg !58
  %18 = add nsw i32 %13, %17, !dbg !58
  %19 = sext i32 %18 to i64, !dbg !58
  %20 = getelementptr inbounds i32, ptr %9, i64 %19, !dbg !58
  store i32 4, ptr %20, align 4, !dbg !58
  %21 = load ptr, ptr %2, align 8, !dbg !58
  %22 = getelementptr inbounds %struct.dynarr, ptr %21, i64 -1, !dbg !58
  %23 = getelementptr inbounds nuw %struct.dynarr, ptr %22, i32 0, i32 3, !dbg !58
  %24 = load i32, ptr %23, align 4, !dbg !58
  %25 = add nsw i32 %24, 1, !dbg !58
  store i32 %25, ptr %23, align 4, !dbg !58
  br label %26, !dbg !58

26:                                               ; preds = %8, %7
  %27 = phi i32 [ -1, %7 ], [ %24, %8 ], !dbg !58
  %28 = call i32 @dynarrGrow(ptr noundef %2), !dbg !59
  %29 = icmp ne i32 %28, 0, !dbg !59
  br i1 %29, label %30, label %31, !dbg !59

30:                                               ; preds = %26
  br label %49, !dbg !59

31:                                               ; preds = %26
  %32 = load ptr, ptr %2, align 8, !dbg !59
  %33 = load ptr, ptr %2, align 8, !dbg !59
  %34 = getelementptr inbounds %struct.dynarr, ptr %33, i64 -1, !dbg !59
  %35 = getelementptr inbounds nuw %struct.dynarr, ptr %34, i32 0, i32 2, !dbg !59
  %36 = load i32, ptr %35, align 4, !dbg !59
  %37 = load ptr, ptr %2, align 8, !dbg !59
  %38 = getelementptr inbounds %struct.dynarr, ptr %37, i64 -1, !dbg !59
  %39 = getelementptr inbounds nuw %struct.dynarr, ptr %38, i32 0, i32 3, !dbg !59
  %40 = load i32, ptr %39, align 4, !dbg !59
  %41 = add nsw i32 %36, %40, !dbg !59
  %42 = sext i32 %41 to i64, !dbg !59
  %43 = getelementptr inbounds i32, ptr %32, i64 %42, !dbg !59
  store i32 1, ptr %43, align 4, !dbg !59
  %44 = load ptr, ptr %2, align 8, !dbg !59
  %45 = getelementptr inbounds %struct.dynarr, ptr %44, i64 -1, !dbg !59
  %46 = getelementptr inbounds nuw %struct.dynarr, ptr %45, i32 0, i32 3, !dbg !59
  %47 = load i32, ptr %46, align 4, !dbg !59
  %48 = add nsw i32 %47, 1, !dbg !59
  store i32 %48, ptr %46, align 4, !dbg !59
  br label %49, !dbg !59

49:                                               ; preds = %31, %30
  %50 = phi i32 [ -1, %30 ], [ %47, %31 ], !dbg !59
  %51 = call i32 @dynarrGrow(ptr noundef %2), !dbg !60
  %52 = icmp ne i32 %51, 0, !dbg !60
  br i1 %52, label %53, label %54, !dbg !60

53:                                               ; preds = %49
  br label %72, !dbg !60

54:                                               ; preds = %49
  %55 = load ptr, ptr %2, align 8, !dbg !60
  %56 = load ptr, ptr %2, align 8, !dbg !60
  %57 = getelementptr inbounds %struct.dynarr, ptr %56, i64 -1, !dbg !60
  %58 = getelementptr inbounds nuw %struct.dynarr, ptr %57, i32 0, i32 2, !dbg !60
  %59 = load i32, ptr %58, align 4, !dbg !60
  %60 = load ptr, ptr %2, align 8, !dbg !60
  %61 = getelementptr inbounds %struct.dynarr, ptr %60, i64 -1, !dbg !60
  %62 = getelementptr inbounds nuw %struct.dynarr, ptr %61, i32 0, i32 3, !dbg !60
  %63 = load i32, ptr %62, align 4, !dbg !60
  %64 = add nsw i32 %59, %63, !dbg !60
  %65 = sext i32 %64 to i64, !dbg !60
  %66 = getelementptr inbounds i32, ptr %55, i64 %65, !dbg !60
  store i32 3, ptr %66, align 4, !dbg !60
  %67 = load ptr, ptr %2, align 8, !dbg !60
  %68 = getelementptr inbounds %struct.dynarr, ptr %67, i64 -1, !dbg !60
  %69 = getelementptr inbounds nuw %struct.dynarr, ptr %68, i32 0, i32 3, !dbg !60
  %70 = load i32, ptr %69, align 4, !dbg !60
  %71 = add nsw i32 %70, 1, !dbg !60
  store i32 %71, ptr %69, align 4, !dbg !60
  br label %72, !dbg !60

72:                                               ; preds = %54, %53
  %73 = phi i32 [ -1, %53 ], [ %70, %54 ], !dbg !60
  %74 = call i32 @dynarrGrow(ptr noundef %2), !dbg !61
  %75 = icmp ne i32 %74, 0, !dbg !61
  br i1 %75, label %76, label %77, !dbg !61

76:                                               ; preds = %72
  br label %95, !dbg !61

77:                                               ; preds = %72
  %78 = load ptr, ptr %2, align 8, !dbg !61
  %79 = load ptr, ptr %2, align 8, !dbg !61
  %80 = getelementptr inbounds %struct.dynarr, ptr %79, i64 -1, !dbg !61
  %81 = getelementptr inbounds nuw %struct.dynarr, ptr %80, i32 0, i32 2, !dbg !61
  %82 = load i32, ptr %81, align 4, !dbg !61
  %83 = load ptr, ptr %2, align 8, !dbg !61
  %84 = getelementptr inbounds %struct.dynarr, ptr %83, i64 -1, !dbg !61
  %85 = getelementptr inbounds nuw %struct.dynarr, ptr %84, i32 0, i32 3, !dbg !61
  %86 = load i32, ptr %85, align 4, !dbg !61
  %87 = add nsw i32 %82, %86, !dbg !61
  %88 = sext i32 %87 to i64, !dbg !61
  %89 = getelementptr inbounds i32, ptr %78, i64 %88, !dbg !61
  store i32 2, ptr %89, align 4, !dbg !61
  %90 = load ptr, ptr %2, align 8, !dbg !61
  %91 = getelementptr inbounds %struct.dynarr, ptr %90, i64 -1, !dbg !61
  %92 = getelementptr inbounds nuw %struct.dynarr, ptr %91, i32 0, i32 3, !dbg !61
  %93 = load i32, ptr %92, align 4, !dbg !61
  %94 = add nsw i32 %93, 1, !dbg !61
  store i32 %94, ptr %92, align 4, !dbg !61
  br label %95, !dbg !61

95:                                               ; preds = %77, %76
  %96 = phi i32 [ -1, %76 ], [ %93, %77 ], !dbg !61
  %97 = load ptr, ptr %2, align 8, !dbg !62
  call void @dynarrSortStandard(ptr noundef %97, ptr noundef @sortInt), !dbg !62
    #dbg_declare(ptr %3, !63, !DIExpression(), !65)
  store i32 0, ptr %3, align 4, !dbg !65
  br label %98, !dbg !66

98:                                               ; preds = %130, %95
  %99 = load i32, ptr %3, align 4, !dbg !67
  %100 = load ptr, ptr %2, align 8, !dbg !69
  %101 = getelementptr inbounds %struct.dynarr, ptr %100, i64 -1, !dbg !69
  %102 = getelementptr inbounds nuw %struct.dynarr, ptr %101, i32 0, i32 3, !dbg !69
  %103 = load i32, ptr %102, align 4, !dbg !69
  %104 = icmp slt i32 %99, %103, !dbg !70
  br i1 %104, label %105, label %133, !dbg !71

105:                                              ; preds = %98
  %106 = load i32, ptr %3, align 4, !dbg !72
  %107 = load i32, ptr %3, align 4, !dbg !73
  %108 = icmp sge i32 %107, 0, !dbg !73
  br i1 %108, label %109, label %117, !dbg !73

109:                                              ; preds = %105
  %110 = load i32, ptr %3, align 4, !dbg !73
  %111 = load ptr, ptr %2, align 8, !dbg !73
  %112 = getelementptr inbounds %struct.dynarr, ptr %111, i64 -1, !dbg !73
  %113 = getelementptr inbounds nuw %struct.dynarr, ptr %112, i32 0, i32 3, !dbg !73
  %114 = load i32, ptr %113, align 4, !dbg !73
  %115 = icmp slt i32 %110, %114, !dbg !73
  br i1 %115, label %116, label %117, !dbg !73

116:                                              ; preds = %109
  br label %118, !dbg !73

117:                                              ; preds = %109, %105
  call void @__assert_fail(ptr noundef @.str.1, ptr noundef @.str.2, i32 noundef 316, ptr noundef @__PRETTY_FUNCTION__.main) #7, !dbg !73
  unreachable, !dbg !73

118:                                              ; preds = %116
  %119 = load ptr, ptr %2, align 8, !dbg !76
  %120 = load ptr, ptr %2, align 8, !dbg !76
  %121 = getelementptr inbounds %struct.dynarr, ptr %120, i64 -1, !dbg !76
  %122 = getelementptr inbounds nuw %struct.dynarr, ptr %121, i32 0, i32 2, !dbg !76
  %123 = load i32, ptr %122, align 4, !dbg !76
  %124 = load i32, ptr %3, align 4, !dbg !76
  %125 = add nsw i32 %123, %124, !dbg !76
  %126 = sext i32 %125 to i64, !dbg !76
  %127 = getelementptr inbounds i32, ptr %119, i64 %126, !dbg !76
  %128 = load i32, ptr %127, align 4, !dbg !76
  %129 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %106, i32 noundef %128), !dbg !77
  br label %130, !dbg !77

130:                                              ; preds = %118
  %131 = load i32, ptr %3, align 4, !dbg !78
  %132 = add nsw i32 %131, 1, !dbg !78
  store i32 %132, ptr %3, align 4, !dbg !78
  br label %98, !dbg !79, !llvm.loop !80

133:                                              ; preds = %98
  %134 = load ptr, ptr %2, align 8, !dbg !83
  call void @dynarrFree(ptr noundef %134), !dbg !83
  ret i32 0, !dbg !84
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal ptr @dynarrNew(i32 noundef %0) #0 !dbg !85 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %0, ptr %3, align 4
    #dbg_declare(ptr %3, !88, !DIExpression(), !89)
    #dbg_declare(ptr %4, !90, !DIExpression(), !91)
  %5 = load i32, ptr %3, align 4, !dbg !92
  %6 = sext i32 %5 to i64, !dbg !92
  %7 = add i64 16, %6, !dbg !92
  %8 = call noalias ptr @calloc(i64 noundef 1, i64 noundef %7) #8, !dbg !92
  store ptr %8, ptr %4, align 8, !dbg !91
  %9 = load ptr, ptr %4, align 8, !dbg !93
  %10 = icmp ne ptr %9, null, !dbg !93
  br i1 %10, label %12, label %11, !dbg !95

11:                                               ; preds = %1
  store ptr null, ptr %2, align 8, !dbg !96
  br label %20, !dbg !96

12:                                               ; preds = %1
  %13 = load ptr, ptr %4, align 8, !dbg !97
  %14 = getelementptr inbounds nuw %struct.dynarr, ptr %13, i32 0, i32 0, !dbg !98
  store i32 1, ptr %14, align 4, !dbg !99
  %15 = load i32, ptr %3, align 4, !dbg !100
  %16 = load ptr, ptr %4, align 8, !dbg !101
  %17 = getelementptr inbounds nuw %struct.dynarr, ptr %16, i32 0, i32 1, !dbg !102
  store i32 %15, ptr %17, align 4, !dbg !103
  %18 = load ptr, ptr %4, align 8, !dbg !104
  %19 = getelementptr inbounds %struct.dynarr, ptr %18, i64 1, !dbg !104
  store ptr %19, ptr %2, align 8, !dbg !105
  br label %20, !dbg !105

20:                                               ; preds = %12, %11
  %21 = load ptr, ptr %2, align 8, !dbg !106
  ret ptr %21, !dbg !106
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal i32 @dynarrGrow(ptr noundef %0) #0 !dbg !107 {
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !110, !DIExpression(), !111)
  %5 = load ptr, ptr %3, align 8, !dbg !112
  %6 = load ptr, ptr %5, align 8, !dbg !112
  %7 = getelementptr inbounds %struct.dynarr, ptr %6, i64 -1, !dbg !112
  %8 = getelementptr inbounds nuw %struct.dynarr, ptr %7, i32 0, i32 2, !dbg !112
  %9 = load i32, ptr %8, align 4, !dbg !112
  %10 = load ptr, ptr %3, align 8, !dbg !114
  %11 = load ptr, ptr %10, align 8, !dbg !114
  %12 = getelementptr inbounds %struct.dynarr, ptr %11, i64 -1, !dbg !114
  %13 = getelementptr inbounds nuw %struct.dynarr, ptr %12, i32 0, i32 3, !dbg !114
  %14 = load i32, ptr %13, align 4, !dbg !114
  %15 = add nsw i32 %9, %14, !dbg !115
  %16 = load ptr, ptr %3, align 8, !dbg !116
  %17 = load ptr, ptr %16, align 8, !dbg !116
  %18 = getelementptr inbounds %struct.dynarr, ptr %17, i64 -1, !dbg !116
  %19 = getelementptr inbounds nuw %struct.dynarr, ptr %18, i32 0, i32 0, !dbg !116
  %20 = load i32, ptr %19, align 4, !dbg !116
  %21 = icmp eq i32 %15, %20, !dbg !117
  br i1 %21, label %22, label %102, !dbg !117

22:                                               ; preds = %1
  %23 = load ptr, ptr %3, align 8, !dbg !118
  %24 = load ptr, ptr %23, align 8, !dbg !118
  %25 = getelementptr inbounds %struct.dynarr, ptr %24, i64 -1, !dbg !118
  %26 = getelementptr inbounds nuw %struct.dynarr, ptr %25, i32 0, i32 2, !dbg !118
  %27 = load i32, ptr %26, align 4, !dbg !118
  %28 = load ptr, ptr %3, align 8, !dbg !121
  %29 = load ptr, ptr %28, align 8, !dbg !121
  %30 = getelementptr inbounds %struct.dynarr, ptr %29, i64 -1, !dbg !121
  %31 = getelementptr inbounds nuw %struct.dynarr, ptr %30, i32 0, i32 3, !dbg !121
  %32 = load i32, ptr %31, align 4, !dbg !121
  %33 = icmp sge i32 %27, %32, !dbg !122
  br i1 %33, label %34, label %69, !dbg !122

34:                                               ; preds = %22
  %35 = load ptr, ptr %3, align 8, !dbg !123
  %36 = load ptr, ptr %35, align 8, !dbg !125
  %37 = load ptr, ptr %3, align 8, !dbg !126
  %38 = load ptr, ptr %37, align 8, !dbg !126
  %39 = load ptr, ptr %3, align 8, !dbg !126
  %40 = load ptr, ptr %39, align 8, !dbg !126
  %41 = getelementptr inbounds %struct.dynarr, ptr %40, i64 -1, !dbg !126
  %42 = getelementptr inbounds nuw %struct.dynarr, ptr %41, i32 0, i32 1, !dbg !126
  %43 = load i32, ptr %42, align 4, !dbg !126
  %44 = load ptr, ptr %3, align 8, !dbg !126
  %45 = load ptr, ptr %44, align 8, !dbg !126
  %46 = getelementptr inbounds %struct.dynarr, ptr %45, i64 -1, !dbg !126
  %47 = getelementptr inbounds nuw %struct.dynarr, ptr %46, i32 0, i32 2, !dbg !126
  %48 = load i32, ptr %47, align 4, !dbg !126
  %49 = add nsw i32 %48, 0, !dbg !126
  %50 = mul nsw i32 %43, %49, !dbg !126
  %51 = sext i32 %50 to i64, !dbg !126
  %52 = getelementptr inbounds i8, ptr %38, i64 %51, !dbg !126
  %53 = load ptr, ptr %3, align 8, !dbg !127
  %54 = load ptr, ptr %53, align 8, !dbg !127
  %55 = getelementptr inbounds %struct.dynarr, ptr %54, i64 -1, !dbg !127
  %56 = getelementptr inbounds nuw %struct.dynarr, ptr %55, i32 0, i32 1, !dbg !127
  %57 = load i32, ptr %56, align 4, !dbg !127
  %58 = load ptr, ptr %3, align 8, !dbg !128
  %59 = load ptr, ptr %58, align 8, !dbg !128
  %60 = getelementptr inbounds %struct.dynarr, ptr %59, i64 -1, !dbg !128
  %61 = getelementptr inbounds nuw %struct.dynarr, ptr %60, i32 0, i32 3, !dbg !128
  %62 = load i32, ptr %61, align 4, !dbg !128
  %63 = mul nsw i32 %57, %62, !dbg !129
  %64 = sext i32 %63 to i64, !dbg !127
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %36, ptr align 1 %52, i64 %64, i1 false), !dbg !130
  %65 = load ptr, ptr %3, align 8, !dbg !131
  %66 = load ptr, ptr %65, align 8, !dbg !131
  %67 = getelementptr inbounds %struct.dynarr, ptr %66, i64 -1, !dbg !131
  %68 = getelementptr inbounds nuw %struct.dynarr, ptr %67, i32 0, i32 2, !dbg !131
  store i32 0, ptr %68, align 4, !dbg !132
  br label %101, !dbg !133

69:                                               ; preds = %22
    #dbg_declare(ptr %4, !134, !DIExpression(), !136)
  %70 = load ptr, ptr %3, align 8, !dbg !137
  %71 = load ptr, ptr %70, align 8, !dbg !137
  %72 = getelementptr inbounds %struct.dynarr, ptr %71, i64 -1, !dbg !137
  %73 = load ptr, ptr %3, align 8, !dbg !137
  %74 = load ptr, ptr %73, align 8, !dbg !137
  %75 = getelementptr inbounds %struct.dynarr, ptr %74, i64 -1, !dbg !137
  %76 = getelementptr inbounds nuw %struct.dynarr, ptr %75, i32 0, i32 1, !dbg !137
  %77 = load i32, ptr %76, align 4, !dbg !137
  %78 = load ptr, ptr %3, align 8, !dbg !137
  %79 = load ptr, ptr %78, align 8, !dbg !137
  %80 = getelementptr inbounds %struct.dynarr, ptr %79, i64 -1, !dbg !137
  %81 = getelementptr inbounds nuw %struct.dynarr, ptr %80, i32 0, i32 0, !dbg !137
  %82 = load i32, ptr %81, align 4, !dbg !137
  %83 = mul nsw i32 %77, %82, !dbg !137
  %84 = mul nsw i32 %83, 2, !dbg !137
  %85 = sext i32 %84 to i64, !dbg !137
  %86 = add i64 16, %85, !dbg !137
  %87 = call ptr @realloc(ptr noundef %72, i64 noundef %86) #9, !dbg !137
  store ptr %87, ptr %4, align 8, !dbg !136
  %88 = load ptr, ptr %4, align 8, !dbg !138
  %89 = icmp ne ptr %88, null, !dbg !138
  br i1 %89, label %91, label %90, !dbg !140

90:                                               ; preds = %69
  store i32 -1, ptr %2, align 4, !dbg !141
  br label %103, !dbg !141

91:                                               ; preds = %69
  %92 = load ptr, ptr %4, align 8, !dbg !142
  %93 = getelementptr inbounds %struct.dynarr, ptr %92, i64 1, !dbg !143
  %94 = load ptr, ptr %3, align 8, !dbg !144
  store ptr %93, ptr %94, align 8, !dbg !145
  %95 = load ptr, ptr %3, align 8, !dbg !146
  %96 = load ptr, ptr %95, align 8, !dbg !146
  %97 = getelementptr inbounds %struct.dynarr, ptr %96, i64 -1, !dbg !146
  %98 = getelementptr inbounds nuw %struct.dynarr, ptr %97, i32 0, i32 0, !dbg !146
  %99 = load i32, ptr %98, align 4, !dbg !147
  %100 = mul nsw i32 %99, 2, !dbg !147
  store i32 %100, ptr %98, align 4, !dbg !147
  br label %101

101:                                              ; preds = %91, %34
  br label %102, !dbg !148

102:                                              ; preds = %101, %1
  store i32 0, ptr %2, align 4, !dbg !149
  br label %103, !dbg !149

103:                                              ; preds = %102, %90
  %104 = load i32, ptr %2, align 4, !dbg !150
  ret i32 %104, !dbg !150
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @dynarrSortStandard(ptr noundef %0, ptr noundef %1) #0 !dbg !151 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !154, !DIExpression(), !155)
  store ptr %1, ptr %4, align 8
    #dbg_declare(ptr %4, !156, !DIExpression(), !157)
  %5 = load ptr, ptr %3, align 8, !dbg !158
  %6 = load ptr, ptr %3, align 8, !dbg !158
  %7 = getelementptr inbounds %struct.dynarr, ptr %6, i64 -1, !dbg !158
  %8 = getelementptr inbounds nuw %struct.dynarr, ptr %7, i32 0, i32 1, !dbg !158
  %9 = load i32, ptr %8, align 4, !dbg !158
  %10 = load ptr, ptr %3, align 8, !dbg !158
  %11 = getelementptr inbounds %struct.dynarr, ptr %10, i64 -1, !dbg !158
  %12 = getelementptr inbounds nuw %struct.dynarr, ptr %11, i32 0, i32 2, !dbg !158
  %13 = load i32, ptr %12, align 4, !dbg !158
  %14 = add nsw i32 %13, 0, !dbg !158
  %15 = mul nsw i32 %9, %14, !dbg !158
  %16 = sext i32 %15 to i64, !dbg !158
  %17 = getelementptr inbounds i8, ptr %5, i64 %16, !dbg !158
  %18 = load ptr, ptr %3, align 8, !dbg !159
  %19 = getelementptr inbounds %struct.dynarr, ptr %18, i64 -1, !dbg !159
  %20 = getelementptr inbounds nuw %struct.dynarr, ptr %19, i32 0, i32 3, !dbg !159
  %21 = load i32, ptr %20, align 4, !dbg !159
  %22 = sext i32 %21 to i64, !dbg !159
  %23 = load ptr, ptr %3, align 8, !dbg !160
  %24 = getelementptr inbounds %struct.dynarr, ptr %23, i64 -1, !dbg !160
  %25 = getelementptr inbounds nuw %struct.dynarr, ptr %24, i32 0, i32 1, !dbg !160
  %26 = load i32, ptr %25, align 4, !dbg !160
  %27 = sext i32 %26 to i64, !dbg !160
  %28 = load ptr, ptr %4, align 8, !dbg !161
  call void @qsort(ptr noundef %17, i64 noundef %22, i64 noundef %27, ptr noundef %28), !dbg !162
  ret void, !dbg !163
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal i32 @sortInt(ptr noundef %0, ptr noundef %1) #0 !dbg !164 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
    #dbg_declare(ptr %3, !169, !DIExpression(), !170)
  store ptr %1, ptr %4, align 8
    #dbg_declare(ptr %4, !171, !DIExpression(), !172)
  %5 = load ptr, ptr %3, align 8, !dbg !173
  %6 = load i32, ptr %5, align 4, !dbg !174
  %7 = load ptr, ptr %4, align 8, !dbg !175
  %8 = load i32, ptr %7, align 4, !dbg !176
  %9 = sub nsw i32 %6, %8, !dbg !177
  ret i32 %9, !dbg !178
}

declare i32 @printf(ptr noundef, ...) #1

; Function Attrs: cold noreturn nounwind
declare void @__assert_fail(ptr noundef, ptr noundef, i32 noundef, ptr noundef) #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @dynarrFree(ptr noundef %0) #0 !dbg !179 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
    #dbg_declare(ptr %2, !182, !DIExpression(), !183)
  %3 = load ptr, ptr %2, align 8, !dbg !184
  %4 = getelementptr inbounds %struct.dynarr, ptr %3, i64 -1, !dbg !184
  call void @free(ptr noundef %4) #10, !dbg !184
  ret void, !dbg !185
}

; Function Attrs: nounwind allocsize(0,1)
declare noalias ptr @calloc(i64 noundef, i64 noundef) #3

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #4

; Function Attrs: nounwind allocsize(1)
declare ptr @realloc(ptr noundef, i64 noundef) #5

declare void @qsort(ptr noundef, i64 noundef, i64 noundef, ptr noundef) #1

; Function Attrs: nounwind
declare void @free(ptr noundef) #6

attributes #0 = { noinline nounwind optnone sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { cold noreturn nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind allocsize(0,1) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #5 = { nounwind allocsize(1) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { cold noreturn nounwind }
attributes #8 = { nounwind allocsize(0,1) }
attributes #9 = { nounwind allocsize(1) }
attributes #10 = { nounwind }

!llvm.dbg.cu = !{!23}
!llvm.module.flags = !{!43, !44, !45, !46, !47, !48, !49}
!llvm.ident = !{!50}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(scope: null, file: !2, line: 316, type: !3, isLocal: true, isDefinition: true)
!2 = !DIFile(filename: "dynarr.c", directory: "/home/sentenced/Documents/Work/UCL/Repos/InformationFlow/tests/third_party", checksumkind: CSK_MD5, checksum: "534f8d7db4306629bef1f276e66d65cc")
!3 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 120, elements: !5)
!4 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!5 = !{!6}
!6 = !DISubrange(count: 15)
!7 = !DIGlobalVariableExpression(var: !8, expr: !DIExpression())
!8 = distinct !DIGlobalVariable(scope: null, file: !2, line: 316, type: !9, isLocal: true, isDefinition: true)
!9 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 448, elements: !10)
!10 = !{!11}
!11 = !DISubrange(count: 56)
!12 = !DIGlobalVariableExpression(var: !13, expr: !DIExpression())
!13 = distinct !DIGlobalVariable(scope: null, file: !2, line: 316, type: !14, isLocal: true, isDefinition: true)
!14 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 72, elements: !15)
!15 = !{!16}
!16 = !DISubrange(count: 9)
!17 = !DIGlobalVariableExpression(var: !18, expr: !DIExpression())
!18 = distinct !DIGlobalVariable(scope: null, file: !2, line: 316, type: !19, isLocal: true, isDefinition: true)
!19 = !DICompositeType(tag: DW_TAG_array_type, baseType: !20, size: 88, elements: !21)
!20 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !4)
!21 = !{!22}
!22 = !DISubrange(count: 11)
!23 = distinct !DICompileUnit(language: DW_LANG_C11, file: !2, producer: "clang version 20.1.8", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, retainedTypes: !24, globals: !42, splitDebugInlining: false, nameTableKind: None)
!24 = !{!25, !27, !29, !36, !28, !41}
!25 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !26, size: 64)
!26 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!27 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !28, size: 64)
!28 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!29 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !30, size: 64)
!30 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "dynarr", file: !2, line: 145, size: 128, elements: !31)
!31 = !{!32, !33, !34, !35}
!32 = !DIDerivedType(tag: DW_TAG_member, name: "capa", scope: !30, file: !2, line: 146, baseType: !26, size: 32)
!33 = !DIDerivedType(tag: DW_TAG_member, name: "elem", scope: !30, file: !2, line: 146, baseType: !26, size: 32, offset: 32)
!34 = !DIDerivedType(tag: DW_TAG_member, name: "offs", scope: !30, file: !2, line: 146, baseType: !26, size: 32, offset: 64)
!35 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !30, file: !2, line: 146, baseType: !26, size: 32, offset: 96)
!36 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !37, size: 64)
!37 = !DISubroutineType(types: !38)
!38 = !{!26, !39, !39}
!39 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !40, size: 64)
!40 = !DIDerivedType(tag: DW_TAG_const_type, baseType: null)
!41 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !4, size: 64)
!42 = !{!0, !7, !12, !17}
!43 = !{i32 7, !"Dwarf Version", i32 5}
!44 = !{i32 2, !"Debug Info Version", i32 3}
!45 = !{i32 1, !"wchar_size", i32 4}
!46 = !{i32 8, !"PIC Level", i32 2}
!47 = !{i32 7, !"PIE Level", i32 2}
!48 = !{i32 7, !"uwtable", i32 2}
!49 = !{i32 7, !"frame-pointer", i32 2}
!50 = !{!"clang version 20.1.8"}
!51 = distinct !DISubprogram(name: "main", scope: !2, file: !2, line: 304, type: !52, scopeLine: 304, spFlags: DISPFlagDefinition, unit: !23, retainedNodes: !54)
!52 = !DISubroutineType(types: !53)
!53 = !{!26}
!54 = !{}
!55 = !DILocalVariable(name: "darr", scope: !51, file: !2, line: 306, type: !25)
!56 = !DILocation(line: 306, column: 10, scope: !51)
!57 = !DILocation(line: 306, column: 17, scope: !51)
!58 = !DILocation(line: 308, column: 5, scope: !51)
!59 = !DILocation(line: 309, column: 5, scope: !51)
!60 = !DILocation(line: 310, column: 5, scope: !51)
!61 = !DILocation(line: 311, column: 5, scope: !51)
!62 = !DILocation(line: 313, column: 5, scope: !51)
!63 = !DILocalVariable(name: "i", scope: !64, file: !2, line: 315, type: !26)
!64 = distinct !DILexicalBlock(scope: !51, file: !2, line: 315, column: 5)
!65 = !DILocation(line: 315, column: 14, scope: !64)
!66 = !DILocation(line: 315, column: 10, scope: !64)
!67 = !DILocation(line: 315, column: 21, scope: !68)
!68 = distinct !DILexicalBlock(scope: !64, file: !2, line: 315, column: 5)
!69 = !DILocation(line: 315, column: 25, scope: !68)
!70 = !DILocation(line: 315, column: 23, scope: !68)
!71 = !DILocation(line: 315, column: 5, scope: !64)
!72 = !DILocation(line: 316, column: 35, scope: !68)
!73 = !DILocation(line: 316, column: 38, scope: !74)
!74 = distinct !DILexicalBlock(scope: !75, file: !2, line: 316, column: 38)
!75 = distinct !DILexicalBlock(scope: !68, file: !2, line: 316, column: 38)
!76 = !DILocation(line: 316, column: 38, scope: !68)
!77 = !DILocation(line: 316, column: 9, scope: !68)
!78 = !DILocation(line: 315, column: 45, scope: !68)
!79 = !DILocation(line: 315, column: 5, scope: !68)
!80 = distinct !{!80, !71, !81, !82}
!81 = !DILocation(line: 316, column: 56, scope: !64)
!82 = !{!"llvm.loop.mustprogress"}
!83 = !DILocation(line: 318, column: 5, scope: !51)
!84 = !DILocation(line: 320, column: 5, scope: !51)
!85 = distinct !DISubprogram(name: "dynarrNew", scope: !2, file: !2, line: 182, type: !86, scopeLine: 182, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !23, retainedNodes: !54)
!86 = !DISubroutineType(types: !87)
!87 = !{!28, !26}
!88 = !DILocalVariable(name: "elem", arg: 1, scope: !85, file: !2, line: 182, type: !26)
!89 = !DILocation(line: 182, column: 30, scope: !85)
!90 = !DILocalVariable(name: "darr", scope: !85, file: !2, line: 184, type: !29)
!91 = !DILocation(line: 184, column: 20, scope: !85)
!92 = !DILocation(line: 184, column: 43, scope: !85)
!93 = !DILocation(line: 186, column: 10, scope: !94)
!94 = distinct !DILexicalBlock(scope: !85, file: !2, line: 186, column: 9)
!95 = !DILocation(line: 186, column: 9, scope: !94)
!96 = !DILocation(line: 186, column: 16, scope: !94)
!97 = !DILocation(line: 188, column: 5, scope: !85)
!98 = !DILocation(line: 188, column: 11, scope: !85)
!99 = !DILocation(line: 188, column: 16, scope: !85)
!100 = !DILocation(line: 189, column: 18, scope: !85)
!101 = !DILocation(line: 189, column: 5, scope: !85)
!102 = !DILocation(line: 189, column: 11, scope: !85)
!103 = !DILocation(line: 189, column: 16, scope: !85)
!104 = !DILocation(line: 191, column: 13, scope: !85)
!105 = !DILocation(line: 191, column: 5, scope: !85)
!106 = !DILocation(line: 192, column: 1, scope: !85)
!107 = distinct !DISubprogram(name: "dynarrGrow", scope: !2, file: !2, line: 196, type: !108, scopeLine: 196, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !23, retainedNodes: !54)
!108 = !DISubroutineType(types: !109)
!109 = !{!26, !27}
!110 = !DILocalVariable(name: "a", arg: 1, scope: !107, file: !2, line: 196, type: !27)
!111 = !DILocation(line: 196, column: 32, scope: !107)
!112 = !DILocation(line: 198, column: 9, scope: !113)
!113 = distinct !DILexicalBlock(scope: !107, file: !2, line: 198, column: 9)
!114 = !DILocation(line: 198, column: 23, scope: !113)
!115 = !DILocation(line: 198, column: 22, scope: !113)
!116 = !DILocation(line: 198, column: 40, scope: !113)
!117 = !DILocation(line: 198, column: 37, scope: !113)
!118 = !DILocation(line: 200, column: 13, scope: !119)
!119 = distinct !DILexicalBlock(scope: !120, file: !2, line: 200, column: 13)
!120 = distinct !DILexicalBlock(scope: !113, file: !2, line: 199, column: 5)
!121 = !DILocation(line: 200, column: 30, scope: !119)
!122 = !DILocation(line: 200, column: 27, scope: !119)
!123 = !DILocation(line: 202, column: 21, scope: !124)
!124 = distinct !DILexicalBlock(scope: !119, file: !2, line: 200, column: 45)
!125 = !DILocation(line: 202, column: 20, scope: !124)
!126 = !DILocation(line: 202, column: 24, scope: !124)
!127 = !DILocation(line: 202, column: 42, scope: !124)
!128 = !DILocation(line: 202, column: 56, scope: !124)
!129 = !DILocation(line: 202, column: 55, scope: !124)
!130 = !DILocation(line: 202, column: 13, scope: !124)
!131 = !DILocation(line: 203, column: 13, scope: !124)
!132 = !DILocation(line: 203, column: 27, scope: !124)
!133 = !DILocation(line: 204, column: 9, scope: !124)
!134 = !DILocalVariable(name: "ptr", scope: !135, file: !2, line: 206, type: !28)
!135 = distinct !DILexicalBlock(scope: !119, file: !2, line: 204, column: 16)
!136 = !DILocation(line: 206, column: 19, scope: !135)
!137 = !DILocation(line: 206, column: 25, scope: !135)
!138 = !DILocation(line: 208, column: 18, scope: !139)
!139 = distinct !DILexicalBlock(scope: !135, file: !2, line: 208, column: 17)
!140 = !DILocation(line: 208, column: 17, scope: !139)
!141 = !DILocation(line: 208, column: 23, scope: !139)
!142 = !DILocation(line: 210, column: 36, scope: !135)
!143 = !DILocation(line: 210, column: 19, scope: !135)
!144 = !DILocation(line: 210, column: 14, scope: !135)
!145 = !DILocation(line: 210, column: 16, scope: !135)
!146 = !DILocation(line: 212, column: 13, scope: !135)
!147 = !DILocation(line: 212, column: 27, scope: !135)
!148 = !DILocation(line: 214, column: 5, scope: !120)
!149 = !DILocation(line: 216, column: 5, scope: !107)
!150 = !DILocation(line: 217, column: 1, scope: !107)
!151 = distinct !DISubprogram(name: "dynarrSortStandard", scope: !2, file: !2, line: 294, type: !152, scopeLine: 294, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !23, retainedNodes: !54)
!152 = !DISubroutineType(types: !153)
!153 = !{null, !28, !36}
!154 = !DILocalVariable(name: "a", arg: 1, scope: !151, file: !2, line: 294, type: !28)
!155 = !DILocation(line: 294, column: 40, scope: !151)
!156 = !DILocalVariable(name: "comp", arg: 2, scope: !151, file: !2, line: 294, type: !36)
!157 = !DILocation(line: 294, column: 48, scope: !151)
!158 = !DILocation(line: 295, column: 11, scope: !151)
!159 = !DILocation(line: 295, column: 28, scope: !151)
!160 = !DILocation(line: 295, column: 42, scope: !151)
!161 = !DILocation(line: 295, column: 56, scope: !151)
!162 = !DILocation(line: 295, column: 5, scope: !151)
!163 = !DILocation(line: 296, column: 1, scope: !151)
!164 = distinct !DISubprogram(name: "sortInt", scope: !2, file: !2, line: 323, type: !165, scopeLine: 323, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !23, retainedNodes: !54)
!165 = !DISubroutineType(types: !166)
!166 = !{!26, !167, !167}
!167 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !168, size: 64)
!168 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !26)
!169 = !DILocalVariable(name: "a", arg: 1, scope: !164, file: !2, line: 323, type: !167)
!170 = !DILocation(line: 323, column: 32, scope: !164)
!171 = !DILocalVariable(name: "b", arg: 2, scope: !164, file: !2, line: 323, type: !167)
!172 = !DILocation(line: 323, column: 46, scope: !164)
!173 = !DILocation(line: 324, column: 13, scope: !164)
!174 = !DILocation(line: 324, column: 12, scope: !164)
!175 = !DILocation(line: 324, column: 16, scope: !164)
!176 = !DILocation(line: 324, column: 15, scope: !164)
!177 = !DILocation(line: 324, column: 14, scope: !164)
!178 = !DILocation(line: 324, column: 5, scope: !164)
!179 = distinct !DISubprogram(name: "dynarrFree", scope: !2, file: !2, line: 193, type: !180, scopeLine: 193, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !23, retainedNodes: !54)
!180 = !DISubroutineType(types: !181)
!181 = !{null, !28}
!182 = !DILocalVariable(name: "a", arg: 1, scope: !179, file: !2, line: 193, type: !28)
!183 = !DILocation(line: 193, column: 32, scope: !179)
!184 = !DILocation(line: 194, column: 5, scope: !179)
!185 = !DILocation(line: 195, column: 1, scope: !179)
