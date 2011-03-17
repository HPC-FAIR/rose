! Rice test
!
module module_bits

contains

!-------------------------------------------------------------------------------
! implicit result type
!-------------------------------------------------------------------------------
  function number_of_bits1(i) result(num)
    num = 0
    itmp = i
    do while (itmp .gt. 0)
       num = num + 1
       itmp = ishft(itmp, -1)
    end do
  end function number_of_bits1


!-------------------------------------------------------------------------------
! result type in function definition
!-------------------------------------------------------------------------------
  integer function number_of_bits2(i) result(num)
  implicit none
  integer :: i, itmp
    num = 0
    itmp = i
    do while (itmp .gt. 0)
       num = num + 1
       itmp = ishft(itmp, -1)
    end do
  end function number_of_bits2

!-------------------------------------------------------------------------------
! result type in type declaration
!-------------------------------------------------------------------------------
  function number_of_bits3(i) result(num)
    implicit none
    integer :: i, itmp, num 
    num = 0
    itmp = i
    do while (itmp .gt. 0)
       num = num + 1
       itmp = ishft(itmp, -1)
    end do
  end function number_of_bits3

!-------------------------------------------------------------------------------
! result type in function declaration
!-------------------------------------------------------------------------------
  integer function number_of_bits4(i) 
  implicit none
  integer :: i, itmp
    number_of_bits4 = 0
    itmp = i
    do while (itmp .gt. 0)
       number_of_bits4 = number_of_bits4 + 1
       itmp = ishft(itmp, -1)
    end do
  end function number_of_bits4


!-------------------------------------------------------------------------------
! result type in type declaration
!-------------------------------------------------------------------------------
  function number_of_bits5(i) 
  implicit none
  integer :: i, itmp, number_of_bits5
    number_of_bits5 = 0
    itmp = i
    do while (itmp .gt. 0)
       number_of_bits5 = number_of_bits5 + 1
       itmp = ishft(itmp, -1)
    end do
  end function number_of_bits5

end module module_bits


program test_module_bits
use module_bits
integer :: error

error = 0
if (4 .ne. number_of_bits1(14)) then
   error = error + 1
endif
if (4 .ne. number_of_bits2(14)) then
   error = error + 1
endif
if (4 .ne. number_of_bits4(14)) then
   error = error + 1
endif
if (4 .ne. number_of_bits4(14)) then
   error = error + 1
endif
if (4 .ne. number_of_bits5(14)) then
   error = error + 1
endif

if (0 .ne. error) then
  write(*, '(A)') "Test failed"
else
  write(*, '(A)') "Test passed"
endif

end program test_module_bits
