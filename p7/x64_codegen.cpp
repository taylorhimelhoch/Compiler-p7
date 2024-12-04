#include <ostream>
#include "3ac.hpp"

namespace cminusminus{

void IRProgram::allocGlobals(){
	//Choose a label for each global
	//TODO(Implement me)
	for(auto g : globals){
		SymOpd * globalOpd = g.second;
		std::string memLoc = "gbl_";
		const SemSymbol * sym = globalOpd->getSym();
		memLoc += sym->getName();
		globalOpd->setMemoryLoc("(" + memLoc + ")");
	}
	for(auto s : strings){
		TODO(Implement me)
	}
}

void IRProgram::datagenX64(std::ostream& out){
	//TODO(Write out data section)
	//Put this directive after you write out strings
	// so that everything is aligned to a quadword value
	// again
	out << ".globl main\n";
	out << ".data\n";
	for(auto g : globals){
		SymOpd * globalOpd = g.second;
		std::string memLoc = "gbl_";
		const SemSymbol * sym = globalOpd->getSym();
		memLoc += sym->getName();
		size_t width = sym->getDataType()->getSize();
		out << memLoc << ":";
		if(width == 8){
			out << ".quad 0\n";
		} else {
			out << ".space " << width << "\n";
		}
	}
	out << ".align 8\n";

}

void IRProgram::toX64(std::ostream& out){
	allocGlobals();
	datagenX64(out);
	// Iterate over each procedure and codegen it
	out<<".text\n";
	//TODO(Implement me)
	for (auto proc : *this->procs){
		proc->toX64(out);
		out << "\n";
	}
}

void Procedure::allocLocals(){
	//Allocate space for locals
	//Iterate over each procedure and codegen it
	//telling each variable where it sits in memory
	//TODO(Implement me)


	for(auto t : temps){
		//TODO(Implement me)
	}
	for(auto t : locals){
		TODO(Implement me)
	}
	for(auto t : formals){
		TODO(Implement me)
	}
	for(auto t : addrOpds){
		//may never happen?
		TODO(Implement me)
	}
}

void Procedure::toX64(std::ostream& out){
	//Allocate all locals
	allocLocals();

	enter->codegenLabels(out);
	enter->codegenX64(out);
	out << "#Fn body " << myName << "\n";
	for (auto quad : *bodyQuads){
		quad->codegenLabels(out);
		out << "#" << quad->toString() << "\n";
		quad->codegenX64(out);
	}
	out << "#Fn epilogue " << myName << "\n";
	leave->codegenLabels(out);
	leave->codegenX64(out);
}

void Quad::codegenLabels(std::ostream& out){
	if (labels.empty()){ return; }

	size_t numLabels = labels.size();
	size_t labelIdx = 0;
	for ( Label * label : labels){
		out << label->getName() << ": ";
		if (labelIdx != numLabels - 1){ out << "\n"; }
		labelIdx++;
	}
}

void BinOpQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
	if(opr == ADD64 || opr == ADD8){
		src1->genLoadVal(out, B);
		src2->genLoadVal(out, A);
		out << "\naddq %rax, %rbx";
		dst->genStoreVal(out, B);
	}
	else if(opr == SUB64 || opr == SUB8){
		src1->genLoadVal(out, B);
		src2->genLoadVal(out, A);
		out << "\nsubq %rax, %rbx";
		dst->genStoreVal(out, B);
	}
	else if(opr == DIV64 || opr == DIV8){
		src1->genLoadVal(out, B);
		src2->genLoadVal(out, A);
		out << "\ndivq %rax, %rbx";
		dst->genStoreVal(out, B);
	}
	else if(opr == MULT64 || opr == MULT8){
		src1->genLoadVal(out, B);
		src2->genLoadVal(out, A);
		out << "\nmultq %rax, %rbx";
		dst->genStoreVal(out, B);
	}
	else if(opr == EQ64 || opr == EQ8){
		src1->genLoadVal(out, B);
		src2->genLoadVal(out, A);
		out << "\neq %rax, %rbx";
		dst->genStoreVal(out, B);
	}
	else if(opr == NEQ64 || opr == NEQ8){
		src1->genLoadVal(out, B);
		src2->genLoadVal(out, A);
		out << "\nneq %rax, %rbx";
		dst->genStoreVal(out, B);
	}
	else if(opr == LT64 || opr == LT8){
		src1->genLoadVal(out, B);
		src2->genLoadVal(out, A);
		out << "\nlt %rax, %rbx";
		dst->genStoreVal(out, B);
	}
	else if(opr == GT64 || opr == GT8){
		src1->genLoadVal(out, B);
		src2->genLoadVal(out, A);
		out << "\ngt %rax, %rbx";
		dst->genStoreVal(out, B);
	}
	else if(opr == LTE64 || opr == LTE8){
		src1->genLoadVal(out, B);
		src2->genLoadVal(out, A);
		out << "\nlte %rax, %rbx";
		dst->genStoreVal(out, B);
	}
	else if(opr == GTE64 || opr == GTE8){
		src1->genLoadVal(out, B);
		src2->genLoadVal(out, A);
		out << "\ngte %rax, %rbx";
		dst->genStoreVal(out, B);
	}
	else if(opr == OR64 || opr == OR8){
		src1->genLoadVal(out, B);
		src2->genLoadVal(out, A);
		out << "\nor %rax, %rbx";
		dst->genStoreVal(out, B);
	}
	else if(opr == AND64 || opr == AND8){
		src1->genLoadVal(out, B);
		src2->genLoadVal(out, A);
		out << "\nand %rax, %rbx";
		dst->genStoreVal(out, B);
	}
}

void UnaryOpQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
	if(op == NEG64 || op == NEG8){
		src->genLoadVal(out, A);
		out << "\nnegq %rax, %rbx";
		dst->genStoreVal(out, B);
	}
	else if(op == NOT64 || op == NOT8){
		src->genLoadVal(out, A);
		out << "\nnotq %rax, %rbx";
		dst->genStoreVal(out, B);
	}
}

void AssignQuad::codegenX64(std::ostream& out){
	src->genLoadVal(out, A);
	dst->genStoreVal(out, A);
}

void GotoQuad::codegenX64(std::ostream& out){
	out << "jmp " << tgt->getName() << "\n";
}

void IfzQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
	cnd-> genLoadVal(out, A);
	out << "cmpq " << cnd->getReg(A) << ", $0" << "\n";
	out << "je " << tgt->getName() << "\n";
}

void NopQuad::codegenX64(std::ostream& out){
	out << "nop" << "\n";
}

void IntrinsicOutputQuad::codegenX64(std::ostream& out){
	if (myType->isBool()){
		myArg->genLoadVal(out, DI);
		out << "callq printBool\n";
	} else if(myType->isInt() || myType->isShort()){
			myArg->genLoadVal(out, DI);
			out <<"callq printInt\n";
		}else if(myType->isString()){
			out << "callq printString\n";
		}

}

void IntrinsicInputQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
	if(myType->isBool()){
		myArg->genLoadVal(out, A);
		out << "callq getBool\n";
	}
	else if(myType->isInt() || myType->isShort()){
		myArg->genLoadVal(out, A);
		out << "callq getInt\n";
	}
	else{
		myArg->genStoreVal(out, A);
		out << "\n";
	}
}

void CallQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
	size_t size= myProc->calleeArgs();
	size_t slack = (16 - (size % 16)) % 16;
	out << "subq $" << slack << ", %rbp\n";
	size += slack;
	myProc->calleeArgs(size);

	out << "callq fun_" << callee->getName() << "\n";
	out << "addq $" << slack << ", %rbp\n";
}

void EnterQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
	out << "pushq " << "%rbp\n";
	out << "movq " << "%rsp, %rbp\n";
	out << "addq " << "$16, %rbp\n";
	out << "subq " << "$" << myProc->arSize() << ", %rsp\n";
}

void LeaveQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
	out << "addq " << "$" << myProc->arSize() << ", %rsp\n";
	out <<"popq %rbp\n";
	out << "retq\n";
}

void SetArgQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
	int i = getIndex();
	if(i == 1){
		opd->genLoadVal(out, DI);
	}
	if(i == 2){
		opd->genLoadVal(out, SI);
	}
	if(i == 3){
		opd->genLoadVal(out, D);
	}
	if(i == 4){
		opd->genLoadVal(out, C);
	}
	if(i == 5){
		opd->genLoadVal(out, R8);
	}
	if(i == 6){
		opd->genLoadVal(out, R9);
	}
	if(i > 6){
		out << "pushq $" << opd->valString() << "\n";
	}
}

void GetArgQuad::codegenX64(std::ostream& out){
	//We don't actually need to do anything here
}

void SetRetQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
	opd->genStoreVal(out ,A);
}

void GetRetQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
	opd->genLoadVal(out, A);
}

void LocQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
	if(srcIsLoc){
		src->genStoreAddr(out, A);
	}else {
		src->genStoreVal(out , A);
	}
	if(tgtIsLoc){
		src->genStoreAddr(out, A );
	}else{
		src->genStoreVal(out, A);
	}
}

void SymOpd::genLoadVal(std::ostream& out, Register reg){
	//TODO(Implement me)
	//FIX ME: worry about the width of the operand

	out << getMovOp() << " " << getMemoryLoc() << ", " << getReg(reg) << "\n";
}

void SymOpd::genStoreVal(std::ostream& out, Register reg){
	//TODO(Implement me)
	out << getMovOp() << getReg(reg) << ", " << getMemoryLoc() << "\n";
}

void SymOpd::genLoadAddr(std::ostream& out, Register reg) {
	//TODO(Implement me if necessary)
	out << "leaq " << getMemoryLoc() << ", " << getReg(reg) << "\n";
}

void AuxOpd::genLoadVal(std::ostream& out, Register reg){
	//TODO(Implement me)
	out << "movq " << getMemoryLoc() << ", " << getReg(reg) << "\n";
}

void AuxOpd::genStoreVal(std::ostream& out, Register reg){
	//TODO(Implement me)
	out << "movq " << getReg(reg) << ", " << getMemoryLoc() << "\n";
}
void AuxOpd::genLoadAddr(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AddrOpd::genStoreVal(std::ostream& out, Register reg){
	//TODO(Implement me)
	out << getMovOp() << getReg(reg) << ", $ " << val << "\n";
 }

void AddrOpd::genLoadVal(std::ostream& out, Register reg){
	//TODO(Implement me)
	out << getMovOp() << " $" << val << ", " << getReg(reg) << "\n";
}

void AddrOpd::genStoreAddr(std::ostream& out, Register reg){
	//TODO(Implement me)
	out << "leaq " << getReg(reg) << ", " << getMemoryLoc() << "\n";
}

void AddrOpd::genLoadAddr(std::ostream & out, Register reg){
	//TODO(Implement me)
	out << "leaq " << getMemoryLoc() << ", " << getReg(reg) << "\n";
}

void LitOpd::genLoadVal(std::ostream & out, Register reg){
	out << getMovOp() << " $" << val << ", " << getReg(reg) << "\n";
}

} //biggest bracket
