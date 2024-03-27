; ModuleID = 'sysy'
source_filename = "sysy"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 2, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 6, i32* %b, align 4
  ret i32 218
}
