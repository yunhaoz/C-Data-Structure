from curricula.grade.shortcuts import *
from curricula.grade.setup.build.common import build_gpp_executable
from curricula.grade.test.correctness.common import wrap_runtime_test, make_stdout_runtime_test
from curricula.grade import INCLUDE_PATH
from curricula.library.files import delete_file

from pathlib import Path


grader = Grader()
root = Path(__file__).absolute().parent


@grader.setup.check(sanity=True)
def check_rational_source(context: Context, resources: dict):
    """Check if the header and implementation are present."""

    source_path = resources["source_path"] = context.problem_directory.joinpath("rational.cpp")
    header_path = resources["header_path"] = context.problem_directory.joinpath("rational.h")
    if source_path.exists() and header_path.exists():
        return CheckResult(passed=True)
    else:
        return CheckResult(passed=False, error="Can't find source and/or header!")


@grader.setup.build(dependency="check_rational_source", sanity=True)
def build_rational_harness(source_path: Path, header_path: Path, resources: dict):
    """Build with harness."""

    executable_path = Path("/tmp", "rational")
    result, resources["executable"] = build_gpp_executable(
        root.joinpath("harness.cpp"),
        source_path,
        destination_path=executable_path,
        gpp_options=("-std=c++11", f"-I{INCLUDE_PATH}", f"-I{header_path.parent}"))
    return result

# The following is what an individual test case would look like without taking
# advantage of:
# - Convenience methods from curricula.grade.test.correctness.common
# - Procedural case generation
#
# @grader.test.correctness(dependency="build_rational_harness", weight=0.5)
# def test_constructor_default(executable: ExecutableFile):
#     runtime = executable.execute("test_constructor_default")
#     if runtime.raised_exception:
#         return CorrectnessResult(passed=False, runtime=runtime.dump(), error=runtime.exception.description)
#     elif runtime.timed_out:
#         return CorrectnessResult(passed=False, runtime=runtime.dump(), error="timed out")
#     elif runtime.code != 0:
#         return CorrectnessResult(passed=False, runtime=runtime.dump(), error="expected status code of zero")
#     elif runtime.stdout != b"0/1":
#         return CorrectnessResult(passed=False, runtime=runtime.dump(), expected="0/1")
#     return CorrectnessResult(passed=True)


def shortcut_executable_stdout_test(test_name: str, weight: float, test_out: bytes):
    grader.test.correctness(
        name=f"test_{test_name}",
        dependency="build_rational_harness",
        weight=weight
    )(wrap_runtime_test(
        executable_name="executable",
        args=(f"test_{test_name}",),
        runtime_test=make_stdout_runtime_test(test_out=test_out)
    ))


shortcut_executable_stdout_test(
    test_name="constructor_default",
    weight=0.5,
    test_out=b"0/1")

shortcut_executable_stdout_test(
    test_name="constructor_basic",
    weight=0.5,
    test_out=b"-1/5")

shortcut_executable_stdout_test(
    test_name="constructor_iostream",
    weight=1.5,
    test_out=b"-9/5 7/3 1/2")

shortcut_executable_stdout_test(
    test_name="addition_rational",
    weight=1,
    test_out=b"-2/1")

shortcut_executable_stdout_test(
    test_name="addition_integral",
    weight=1,
    test_out=b"4/5 4/5")

shortcut_executable_stdout_test(
    test_name="multiplication_rational",
    weight=1,
    test_out=b"9/25")

shortcut_executable_stdout_test(
    test_name="multiplication_integral",
    weight=1,
    test_out=b"-2/5 -2/5")

shortcut_executable_stdout_test(
    test_name="exponent_positive",
    weight=0.5,
    test_out=b"-1/125")

shortcut_executable_stdout_test(
    test_name="exponent_zero",
    weight=0.5,
    test_out=b"1/1")

shortcut_executable_stdout_test(
    test_name="exponent_negative",
    weight=0.5,
    test_out=b"-125/1")

shortcut_executable_stdout_test(
    test_name="equality",
    weight=1,
    test_out=b"1 0")

shortcut_executable_stdout_test(
    test_name="inequality",
    weight=1,
    test_out=b"0 1")

shortcut_executable_stdout_test(
    test_name="less",
    weight=1,
    test_out=b"1 0")

shortcut_executable_stdout_test(
    test_name="assignment_addition_rational",
    weight=0.5,
    test_out=b"-2/1")

shortcut_executable_stdout_test(
    test_name="assignment_addition_integral",
    weight=0.5,
    test_out=b"4/5")

shortcut_executable_stdout_test(
    test_name="assignment_multiplication_rational",
    weight=0.5,
    test_out=b"9/25")

shortcut_executable_stdout_test(
    test_name="assignment_multiplication_integral",
    weight=0.5,
    test_out=b"-2/5")

shortcut_executable_stdout_test(
    test_name="normalized_zero",
    weight=2,
    test_out=b"0/1")

shortcut_executable_stdout_test(
    test_name="addition_with_reduction",
    weight=1,
    test_out=b"1/1")

shortcut_executable_stdout_test(
    test_name="multiplication_with_reduction",
    weight=1,
    test_out=b"1/1")

shortcut_executable_stdout_test(
    test_name="assignment_addition_with_reduction",
    weight=0.5,
    test_out=b"1/1")

shortcut_executable_stdout_test(
    test_name="assignment_multiplication_with_reduction",
    weight=0.5,
    test_out=b"1/1")

shortcut_executable_stdout_test(
    test_name="assignment_addition_same",
    weight=1,
    test_out=b"-2/5")

shortcut_executable_stdout_test(
    test_name="constructor_zero",
    weight=1,
    test_out=b"0/1")


@grader.teardown.cleanup(dependency="build_rational_harness", sanity=True)
def cleanup_rational(executable: ExecutableFile):
    """Remove binary."""

    delete_file(executable.path)
