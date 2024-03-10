; ModuleID = 'sysy'
source_filename = "sysy"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 132, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 2, i32* %b, align 4
  %loadtmp = load i32, i32* %b, align 4
  %0 = add i32 %loadtmp, 1
  %loadtmp1 = load i32, i32* %b, align 4
  %1 = mul i32 %0, %loadtmp1
  %2 = add i32 %1, 132
  ret i32 %2
}
