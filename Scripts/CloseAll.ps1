

# get Resolume process
$resolume = Get-Process Arena -ErrorAction SilentlyContinue
if ($resolume) {
  # try gracefully first
  $resolume.CloseMainWindow()
  # kill after five seconds
  Sleep 5
  if (!$resolume.HasExited) {
    $resolume | Stop-Process -resolume
  }
}
Remove-Variable resolume

# get Ableton process
$ableton = Get-Process ableton -ErrorAction SilentlyContinue
if ($ableton) {
  # try gracefully first
  $ableton.CloseMainWindow()
  # kill after five seconds
  Sleep 5
  if (!$ableton.HasExited) {
    $ableton | Stop-Process -Force
  }
}
Remove-Variable ableton

# get CoralSoulInteractive process
$unity = Get-Process Interactive_Waterfall_020218Interactive -ErrorAction SilentlyContinue
if ($unity) {
  # try gracefully first
  $unity.CloseMainWindow()
  # kill after five seconds
  Sleep 5
  if (!$unity.HasExited) {
    $unity | Stop-Process -Force
  }
}
Remove-Variable unity

# get CoralSoulInteractive process
$master = Get-Process StateMachine -ErrorAction SilentlyContinue
if ($master) {
  # try gracefully first
  $master.CloseMainWindow()
  # kill after five seconds
  Sleep 5
  if (!$master.HasExited) {
    $master | Stop-Process -Force
  }
}
Remove-Variable master


# get CoralSoulApp process
$of = Get-Process CoralSoulApp -ErrorAction SilentlyContinue
if ($of) {
  # try gracefully first
  $of.CloseMainWindow()
  # kill after five seconds
  Sleep 5
  if (!$of.HasExited) {
    $of | Stop-Process -Force
  }
}
Remove-Variable of