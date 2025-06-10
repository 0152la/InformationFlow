; ModuleID = './addll.cpp'
source_filename = "./addll.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Unary Operations

define noundef double @llvm_impl_fneg(double noundef %a)
{
    %x = fneg double %a
    ret double %x
}

; Binary Operations - Integer

define noundef i64 @llvm_impl_add(i64 noundef %a, i64 noundef %b)
{
    %x = add i64 %a, %b
    ret i64 %x
}

define noundef i8 @llvm_impl_add_i8(i8 noundef %a, i8 noundef %b)
{
    %x = add i8 %a, %b
    ret i8 %x
}

define noundef i64 @llvm_impl_sub(i64 noundef %a, i64 noundef %b)
{
    %x = sub i64 %a, %b
    ret i64 %x
}

define noundef i64 @llvm_impl_mul(i64 noundef %a, i64 noundef %b)
{
    %x = mul i64 %a, %b
    ret i64 %x
}

define noundef i64 @llvm_impl_udiv(i64 noundef %a, i64 noundef %b)
{
    %x = udiv i64 %a, %b
    ret i64 %x
}

define noundef i64 @llvm_impl_sdiv(i64 noundef %a, i64 noundef %b)
{
    %x = sdiv i64 %a, %b
    ret i64 %x
}

define noundef i64 @llvm_impl_urem(i64 noundef %a, i64 noundef %b)
{
    %x = urem i64 %a, %b
    ret i64 %x
}

define noundef i64 @llvm_impl_srem(i64 noundef %a, i64 noundef %b)
{
    %x = srem i64 %a, %b
    ret i64 %x
}
