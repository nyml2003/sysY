; ModuleID = 'sysy'
source_filename = "sysy"

@a = global i32 1, align 4
@b = global i32 2, align 4

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 233, i32* %a, align 4
  ret i32 3
}
