; ModuleID = 'sysy'
source_filename = "sysy"

@b = global i32 2, align 4
@a = global i32 123, align 4

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, i32* %a, align 4
  %loadtmp = load i32, i32* %a, align 4
  %0 = add i32 %loadtmp, 4
  ret i32 %0
}
