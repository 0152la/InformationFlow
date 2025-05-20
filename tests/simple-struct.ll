; ModuleID = './struct.cpp'
source_filename = "./struct.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

module asm ".globl _ZSt21ios_base_library_initv"

%struct.D = type { i32, i32 }
%"class.std::basic_ostream" = type { ptr, %"class.std::basic_ios" }
%"class.std::basic_ios" = type { %"class.std::ios_base", ptr, i8, i8, ptr, ptr, ptr, ptr }
%"class.std::ios_base" = type { ptr, i64, i64, i32, i32, i32, ptr, %"struct.std::ios_base::_Words", [8 x %"struct.std::ios_base::_Words"], i32, ptr, %"class.std::locale" }
%"struct.std::ios_base::_Words" = type { ptr, i64 }
%"class.std::locale" = type { ptr }

@__const.main.me = private unnamed_addr constant %struct.D { i32 42, i32 24 }, align 4
@_ZSt4cout = external global %"class.std::basic_ostream", align 8
@.str = private unnamed_addr constant [11 x i8] c"me == x - \00", align 1
@.str.1 = private unnamed_addr constant [8 x i8] c" = y - \00", align 1

; Function Attrs: mustprogress noinline norecurse optnone sspstrong uwtable
define dso_local noundef i32 @main() #0 {
  ;%my_str0 = insertvalue {i32, i32, {i8}} {i32 0, i32 21, {i8 8}}, i32 22, 0
  %my_str0 = insertvalue {i32, i32, {i8, i4}} poison, i32 22, 0
  %my_str1 = insertvalue {i32, i32, {i8, i4}} %my_str0, i8 4, 2, 0
  %my_str2 = insertvalue {i32, i32, {i8, i4}} %my_str1, i4 1, 2, 1

  %1 = alloca i32, align 4
  %2 = alloca %struct.D, align 4
  store i32 0, ptr %1, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr align 4 %2, ptr align 4 @__const.main.me, i64 8, i1 false)
  %3 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(ptr noundef nonnull align 8 dereferenceable(8) @_ZSt4cout, ptr noundef @.str)

  ; Access and write struct element at {0}
  %str_pr_0 = extractvalue {i32, i32, {i8, i4}} %my_str2, 0
  %str_pr_0_1 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZNSolsEi(ptr noundef nonnull align 8 dereferenceable(8) %3, i32 noundef %str_pr_0)
  %str_pr_0_2 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(ptr noundef nonnull align 8 dereferenceable(8) %str_pr_0_1, ptr noundef @.str.1)

  ; Access and write struct element at {2, 0}
  %str_pr_1 = extractvalue {i32, i32, {i8, i4}} %my_str2, 2, 0
  %str_pr_1_1 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZNSolsEi(ptr noundef nonnull align 8 dereferenceable(8) %str_pr_0_2, i8 noundef %str_pr_1)
  %str_pr_1_2 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c(ptr noundef nonnull align 8 dereferenceable(8) %str_pr_1_1, i8 noundef signext 10)

  ; Access and write struct element at {2, 1}
  %str_pr_2 = extractvalue {i32, i32, {i8, i4}} %my_str2, 2, 1
  %str_pr_2_1 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZNSolsEi(ptr noundef nonnull align 8 dereferenceable(8) %str_pr_1_2, i4 noundef %str_pr_2)
  %str_pr_2_2 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c(ptr noundef nonnull align 8 dereferenceable(8) %str_pr_2_1, i8 noundef signext 10)

  ; Access and write array element at {1, 1, 1}
  %my_arr0 = insertvalue [3 x [2 x [15 x i8]]] undef, i8 44, 1, 1, 1
  %arr_pr_0 = extractvalue [3 x [2 x [15 x i8]]] %my_arr0, 1, 1, 1
  %arr_pr_0_1 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZNSolsEi(ptr noundef nonnull align 8 dereferenceable(8) %str_pr_2_2, i8 noundef %arr_pr_0)
  %arr_pr_0_2 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c(ptr noundef nonnull align 8 dereferenceable(8) %arr_pr_0_1, i8 noundef signext 10)

  ; Access and write array element at {1} - resolves to {1, 0, 0}
  ; - minus all the type wrongness...
  %my_arr1 = insertvalue [3 x [2 x [15 x i8]]] %my_arr0, i8 66, 1, 0, 0
  %arr_pr_1 = extractvalue [3 x [2 x [15 x i8]]] %my_arr1, 1
  %arr_pr_1_1 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZNSolsEi(ptr noundef nonnull align 8 dereferenceable(8) %arr_pr_0_1, [2 x [15 x i8]] noundef %arr_pr_1)
  %arr_pr_1_2 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c(ptr noundef nonnull align 8 dereferenceable(8) %arr_pr_1_1, i8 noundef signext 10)

  ret i32 0
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c(ptr noundef nonnull align 8 dereferenceable(8), i8 noundef signext) #2

declare noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(ptr noundef nonnull align 8 dereferenceable(8), ptr noundef) #2

declare noundef nonnull align 8 dereferenceable(8) ptr @_ZNSolsEi(ptr noundef nonnull align 8 dereferenceable(8), i32 noundef) #2

attributes #0 = { mustprogress noinline norecurse optnone sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 19.1.7"}
