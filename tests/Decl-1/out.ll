; ModuleID = 'sysy'
source_filename = "sysy"

@a = constant i32 123, align 4

define i32 @main() {
entry:
  %b = alloca i32, align 4
  store i32 15129, i32* %b, align 4
  ret i32 246
}
