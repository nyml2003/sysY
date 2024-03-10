; ModuleID = 'sysy'
source_filename = "sysy"

@a = external global i32, align 4

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  ret i32 0
}
