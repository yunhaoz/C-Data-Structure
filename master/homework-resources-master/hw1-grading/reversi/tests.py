from curricula.grade.shortcuts import *
from curricula.grade.setup.build.common import build_gpp_executable
from curricula.grade.test.correctness.common import (
    make_exit_zero,
    make_stdout_runtime_test,
    wrap_runtime_test,
    test_runtime_succeeded)
from curricula.grade import INCLUDE_PATH
from curricula.library import valgrind
from curricula.library import files

from pathlib import Path
from typing import Tuple

grader = Grader()
root = Path(__file__).absolute().parent


@grader.setup.check(sanity=True)
def check_reversi_source(context: Context, resources: dict):
    source_path = resources["source_path"] = context.problem_directory.joinpath("reversi.cpp")
    header_path = resources["header_path"] = context.problem_directory.joinpath("reversi.h")
    game_path = resources["game_path"] = context.problem_directory.joinpath("test-reversi.cpp")
    if all(map(Path.exists, (source_path, header_path, game_path))):
        return CheckResult(passed=True)
    else:
        return CheckResult(passed=False, error="Can't find source and/or header!")


@grader.setup.build(sanity=True, dependency="check_reversi_source")
def build_reversi_harness(source_path: Path, header_path: Path, resources: dict):
    harness_executable_path = Path("/tmp", "reversi_harness")
    result, resources["harness_executable"] = build_gpp_executable(
        root.joinpath("harness.cpp"),
        source_path,
        destination_path=harness_executable_path,
        gpp_options=("-std=c++11", f"-I{INCLUDE_PATH}", f"-I{header_path.parent}"))
    return result


grader.test.correctness(
    dependency="build_reversi_harness",
    name="test_square_assignment"
)(make_exit_zero(executable_name="harness_executable", args=("test_square_assignment",)))

grader.test.correctness(
    dependency="build_reversi_harness",
    name="test_square_equal"
)(make_exit_zero(executable_name="harness_executable", args=("test_square_equal",)))

grader.test.correctness(
    dependency="build_reversi_harness",
    name="test_square_unequal"
)(make_exit_zero(executable_name="harness_executable", args=("test_square_unequal",)))

grader.test.correctness(
    dependency="build_reversi_harness",
    name="test_square_output"
)(wrap_runtime_test(
    executable_name="harness_executable",
    args=("test_square_output",),
    runtime_test=make_stdout_runtime_test(test_out_lines=[b"W", b"B"])
))

grader.test.correctness(
    dependency="build_reversi_harness",
    name="test_board_copy"
)(make_exit_zero(executable_name="harness_executable", args=("test_board_copy",)))

EXPECTED_BOARD = b"""\
      
  1234
a:----
b:-BW-
c:-WB-
d:----
"""

grader.test.correctness(
    dependency="build_reversi_harness",
    name="test_board_output"
)(wrap_runtime_test(
    executable_name="harness_executable",
    args=("test_board_output",),
    runtime_test=make_stdout_runtime_test(test_out=EXPECTED_BOARD + EXPECTED_BOARD)
))


@grader.setup.build(sanity=True)
def build_reversi_game(source_path: Path, game_path: Path, resources: dict):
    """Build the actual reversi game."""

    game_executable_path = Path("/tmp", "reversi_game")
    result, resources["game_executable"] = build_gpp_executable(
        source_path,
        game_path,
        destination_path=game_executable_path,
        gpp_options=("-std=c++11",))
    return result


def test_piece_count(name: str, stdin: bytes, results: Tuple[bytes, bytes]):
    """Just check for Win/Tie/Loss and W=#/B=#"""

    @grader.test.correctness(dependency="build_reversi_game", name=name, weight=2)
    def test(game_executable: ExecutableFile):
        runtime = game_executable.execute(stdin=stdin, timeout=2)
        result = test_runtime_succeeded(runtime)
        if not result.passed:
            return result
        lines = runtime.stdout.split(b"\n")
        passed = all(result in lines for result in results)
        if not passed:
            return CorrectnessResult(
                passed=passed,
                expected=b"\n".join(results).decode(errors="replace"),
                runtime=runtime.dump())
        return CorrectnessResult(passed=passed, runtime=runtime.dump())


test_piece_count(
    name="test_game_quit",
    stdin=b"q\n",
    results=(b"Tie", b"W=2/B=2"))

test_piece_count(
    name="test_game_single_capture",
    stdin=b"p a3\nq\n",
    results=(b"B wins", b"W=1/B=4"))

test_piece_count(
    name="test_game_play_1",
    stdin=b"p d2\np d3\np d4\nq\n",
    results=(b"B wins", b"W=1/B=6"))

test_piece_count(
    name="test_game_play_2",
    stdin=b"p c1\np b1\np a1\nq\n",
    results=(b"B wins", b"W=1/B=6"))


test_piece_count(
    name="test_game_play_3",
    stdin=b"p a3\np a2\np a1\nq\n",
    results=(b"B wins", b"W=1/B=6"))

test_piece_count(
    name="test_game_play_4",
    stdin=b"p b4\np c4\np d4\nq\n",
    results=(b"B wins", b"W=1/B=6"))

test_piece_count(
    name="test_game_undo",
    stdin=b"c\np a3\nu\nq\n",
    results=(b"Tie", b"W=2/B=2"))

grader.test.correctness(
    dependency="build_reversi_game",
    name="test_game_undo_empty",
    weight=2
)(make_exit_zero(
    executable_name="game_executable",
    stdin=b"u\nq\n",
    timeout=2,
))


@grader.test.memory(dependency="build_reversi_game", weight=2)
def test_memory_game_single_capture(game_executable: ExecutableFile):
    return MemoryResult.from_valgrind_report(valgrind.run(*game_executable.args, stdin=b"p a3\nq\n", timeout=20))


@grader.teardown.cleanup(sanity=True, dependency="build_reversi_harness")
def cleanup_reversi_harness(harness_executable: ExecutableFile):
    files.delete_file(harness_executable.path)
